#include "Object3D.h"

lcf::Object3D::Object3D(const Object3D &other) :
    // Hierarchical(other), 
    m_transformer(other.m_transformer),
    m_name(other.m_name)
{
}

void lcf::Object3D::draw()
{
}

void lcf::Object3D::drawShadow(LightType light_type)
{
}

lcf::Object3DType lcf::Object3D::type() const
{
    return Object3DType::Group;
}

void lcf::Object3D::attachTo(Object3D *parent)
{
    m_transformer.attachTo(parent ? &parent->m_transformer : nullptr);
}

void lcf::Object3D::setLocalMatrix(const Matrix4x4 & matrix)
{
    m_transformer.setMatrix(matrix);
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
    m_transformer.translate(translation);
}

void lcf::Object3D::setTranslation(float x, float y, float z)
{
    this->setTranslation(Vector3D(x, y, z));
}

void lcf::Object3D::setTranslation(const Vector3D &position)
{
    m_transformer.setPosition(position);
}

void lcf::Object3D::rotate(const Quaternion &rotation)
{
    m_transformer.rotate(rotation);
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
    m_transformer.setRotation(rotation);
}

void lcf::Object3D::rotateX(float angle_deg)
{
    m_transformer.rotateXAxis(angle_deg);
}

void lcf::Object3D::setRotationX(float angle_deg)
{
    m_transformer.setRotationXAxis(angle_deg);
}

void lcf::Object3D::rotateY(float angle_deg)
{
    m_transformer.rotateYAxis(angle_deg);
}

void lcf::Object3D::setRotationY(float angle_deg)
{
    m_transformer.setRotationYAxis(angle_deg);
}

void lcf::Object3D::rotateZ(float angle_deg)
{
    m_transformer.rotateZAxis(angle_deg);
}

void lcf::Object3D::setRotationZ(float angle_deg)
{
    m_transformer.setRotationZAxis(angle_deg);
}

void lcf::Object3D::scale(float x, float y, float z)
{
    this->scale(Vector3D(x, y, z));
}

void lcf::Object3D::scale(float factor)
{
    this->scale(Vector3D(factor, factor, factor));
}

void lcf::Object3D::scale(const Vector3D &scale)
{
    m_transformer.scale(scale);
}

void lcf::Object3D::setScale(float x, float y, float z)
{
    this->setScale(Vector3D(x, y, z));
}

void lcf::Object3D::setScale(const Vector3D & scale)
{
    m_transformer.setScale(scale);
}

void lcf::Object3D::setScale(float factor)
{
    this->setScale(factor, factor, factor);
}

const lcf::Vector3D &lcf::Object3D::localPosition() const
{
    return m_transformer.getPosition();
}

lcf::Vector3D lcf::Object3D::worldPosition()
{
    return m_transformer.getHierarchialPosition();
}

const lcf::Vector3D &lcf::Object3D::translation() const
{
    return this->localPosition();
}

const lcf::Quaternion &lcf::Object3D::rotation() const
{
    return m_transformer.getRotation();
}

const lcf::Vector3D &lcf::Object3D::scale() const
{
    return m_transformer.getScale();
}

const lcf::Matrix4x4 &lcf::Object3D::worldMatrix()
{
    return m_transformer.getHierarchialMatrix();
}

const lcf::Matrix4x4 &lcf::Object3D::inversedWorldMatrix()
{
    return m_transformer.getInvertedHierarchialMatrix();
}

lcf::Matrix3x3 lcf::Object3D::normalMatrix()
{
    return this->inversedWorldMatrix().transposed().toGenericMatrix<3, 3>();
}

void lcf::Object3D::setName(std::string_view name)
{
    m_name = name;
}

const std::string &lcf::Object3D::name() const
{
    return m_name;
}