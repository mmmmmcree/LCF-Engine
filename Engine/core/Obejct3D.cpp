#include "Obejct3D.h"

lcf::Object3D::Object3D(const Object3D &other) :
    m_local(other.m_local),
    m_world(other.m_world),
    m_world_need_update(other.m_world_need_update),
    m_local_decomposed(other.m_local_decomposed),
    m_children(),
    m_parent(nullptr),
    m_name(other.m_name)
{
}

void lcf::Object3D::draw()
{
    for (auto child : m_children) {
        child->draw();
    }
}

lcf::Object3D::Type lcf::Object3D::type() const
{
    return Object3D::Type::Group;
}

void lcf::Object3D::debugParent() const
{
    const Object3D *p = this;
    while (p) {
        qDebug() << "address: " << p << "type: " << static_cast<int>(p->type()) << "parent: " << p->m_parent;
        p = p->m_parent;
    }
}

lcf::Object3D *lcf::Object3D::root() const
{
    Object3D *p = m_parent; 
    while (p) {
        p = p->m_parent;
    }
    return p;
}

void lcf::Object3D::setParent(Object3D *parent)
{
    if (m_parent == parent) { return; }
    if (m_parent) {
        m_parent->removeChild(this);
    }
    m_parent = parent;
    if (not parent) { return; }
    parent->addChildToChildren(this);
}

void lcf::Object3D::addChild(Object3D *child)
{
    if (not child or child == this) { return; }
    child->setParent(this);
}

void lcf::Object3D::setLocalMatrix(const Matrix4x4 & matrix)
{
    m_local = matrix;
    m_local_decomposed = decompose(m_local);
    this->updateWorldMatrix();
}

void lcf::Object3D::translate(float x, float y, float z)
{
    this->translate(Vector3D(x, y, z));
}

void lcf::Object3D::translateX(float x)
{
    this->translate(Vector3D(x, 0.0f, 0.0f));
}

void lcf::Object3D::translateY(float y)
{
    this->translate(Vector3D(0.0f, y, 0.0f));
}

void lcf::Object3D::translateZ(float z)
{
    this->translate(Vector3D(0.0f, 0.0f, z));
}

void lcf::Object3D::translate(const Vector3D &translation)
{
    m_local.translate(translation);
    m_local_decomposed.translation += translation;
    this->updateWorldMatrix();
}

void lcf::Object3D::setTranslation(float x, float y, float z)
{
    this->setTranslation(Vector3D(x, y, z));
}

void lcf::Object3D::setTranslation(const Vector3D &translation)
{
    m_local_decomposed.translation = translation;
    m_local = m_local_decomposed.toTransform();
    this->updateWorldMatrix();
}

void lcf::Object3D::rotate(const Quaternion &rotation)
{
    m_local.rotate(rotation);
    m_local_decomposed.rotation *= rotation;
    m_local_decomposed.rotation.normalize();
    this->updateWorldMatrix();
}

void lcf::Object3D::rotate(float angle_deg, float x, float y, float z)
{
    this->rotate(Quaternion::fromAxisAndAngle(x, y, z, angle_deg));
}

void lcf::Object3D::setRotation(float angle_deg, float x, float y, float z)
{
    this->setRotation(Quaternion::fromAxisAndAngle(x, y, z, angle_deg));
}

void lcf::Object3D::setRotation(const Quaternion &rotation)
{
    m_local_decomposed.rotation = rotation.normalized();
    m_local = m_local_decomposed.toTransform();
    this->updateWorldMatrix();
}

void lcf::Object3D::rotateX(float angle_deg)
{
    this->rotate(Quaternion::fromAxisAndAngle(m_local.column(0).toVector3D(), angle_deg));
}

void lcf::Object3D::setRotationX(float angle_deg)
{
    this->setRotation(Quaternion::fromAxisAndAngle(m_local.column(0).toVector3D(), angle_deg));
}

void lcf::Object3D::rotateY(float angle_deg)
{
    this->rotate(Quaternion::fromAxisAndAngle(m_local.column(1).toVector3D(), angle_deg));
}

void lcf::Object3D::setRotationY(float angle_deg)
{
    this->setRotation(Quaternion::fromAxisAndAngle(m_local.column(1).toVector3D(), angle_deg));
}

void lcf::Object3D::rotateZ(float angle_deg)
{
    this->rotate(Quaternion::fromAxisAndAngle(m_local.column(2).toVector3D(), angle_deg));
}

void lcf::Object3D::setRotationZ(float angle_deg)
{
    this->setRotation(Quaternion::fromAxisAndAngle(m_local.column(2).toVector3D(), angle_deg));
}

void lcf::Object3D::scale(float x, float y, float z)
{
    this->scale(Vector3D(x, y, z));
}

void lcf::Object3D::scale(float factor)
{
    m_local.scale(factor);
    m_local_decomposed.scale *= factor;
    this->updateWorldMatrix();
}

void lcf::Object3D::scale(const Vector3D &scale)
{
    m_local.scale(scale);
    m_local_decomposed.scale *= scale;
    this->updateWorldMatrix();
}

void lcf::Object3D::setScale(float x, float y, float z)
{
    this->setScale(Vector3D(x, y, z));
}

void lcf::Object3D::setScale(const Vector3D & scale)
{
    m_local_decomposed.scale = scale;
    m_local = m_local_decomposed.toTransform();
    this->updateWorldMatrix();
}

void lcf::Object3D::setScale(float factor)
{
    this->setScale(factor, factor, factor);
}

const lcf::Vector3D &lcf::Object3D::position() const
{
    return m_local_decomposed.translation;
}

const std::vector<lcf::Object3D *> &lcf::Object3D::children() const
{
    return m_children;
}

const lcf::Matrix4x4 &lcf::Object3D::worldMatrix()
{
    /*
    - 如果此节点需要更新，则用其父节点的 world_matrix 更新该节点
    - 向上更新时遇到不需要更新的节点就停止，world_matrix 向下传递
    - 更新完毕后此节点的更新需求已经被处理，下次调用 worldMatrix() 就不会再更新
    - 否则代表此节点无需更新，直接返回世界矩阵即可
    */
    if (not m_world_need_update) { return m_world; }
    m_world = m_parent ? m_parent->worldMatrix() * m_local : m_local;
    m_world_need_update = false;
    return m_world;
}

lcf::Matrix3x3 lcf::Object3D::normalMatrix()
{
    return this->worldMatrix().inverted().transposed().toGenericMatrix<3, 3>();
}

void lcf::Object3D::setName(std::string_view name)
{
    m_name = name;
}

const std::string &lcf::Object3D::name() const
{
    return m_name;
}

void lcf::Object3D::updateWorldMatrix()
{
    /*
    - 不处理矩阵更新，只向子结点传递更新需求，并标记自己需要更新
    - 如果自己已经被标记，说明已经向下传递过更新需求，并且该需求没有被处理，不需要再向下传递
    */
    if (m_world_need_update) { return; }
    m_world_need_update = true;
    this->notifyWorldMatrixUpdatedToChildren();
}

void lcf::Object3D::notifyWorldMatrixUpdatedToChildren()
{
    for (auto child : m_children) {
        if (child->m_world_need_update) { continue; }
        child->m_world_need_update = true;
        child->notifyWorldMatrixUpdatedToChildren();
    }
}

void lcf::Object3D::addChildToChildren(Object3D *child)
{
    m_children.push_back(child);
}

void lcf::Object3D::removeChild(Object3D *child)
{
    auto iter = std::find(m_children.begin(), m_children.end(), child);
    if (iter != m_children.end()) {
        m_children.erase(iter);
    }
}