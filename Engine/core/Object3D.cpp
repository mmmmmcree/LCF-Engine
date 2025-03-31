#include "Object3D.h"

lcf::Object3D::Object3D(const Object3D &other) :
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
    m_transformer.setLocalMatrix(matrix);
}

void lcf::Object3D::translateWorld(float x, float y, float z)
{
    this->translateWorld(Vector3D(x, y, z));
}

void lcf::Object3D::translateLocalXAxis(float x)
{
    m_transformer.translateLocalXAxis(x);
}

void lcf::Object3D::translateLocalYAxis(float y)
{
    m_transformer.translateLocalYAxis(y);
}

void lcf::Object3D::translateLocalZAxis(float z)
{
    m_transformer.translateLocalZAxis(z);
}

void lcf::Object3D::setTranslation(const Vector3D &translation)
{
    m_transformer.setTranslation(translation);
}

void lcf::Object3D::setRotation(const Quaternion &rotation)
{
    m_transformer.setRotation(rotation);
}

void lcf::Object3D::setRotation(float angle_deg, float x, float y, float z)
{
    m_transformer.setRotation(angle_deg, x, y, z);
}

void lcf::Object3D::setScale(float x, float y, float z)
{
    m_transformer.setScale(x, y, z);
}

void lcf::Object3D::setScale(const Vector3D & scale)
{
    m_transformer.setScale(scale);
}

void lcf::Object3D::translateWorld(const Vector3D &translation)
{
    m_transformer.translateWorld(translation);
}

void lcf::Object3D::rotateLocal(const Quaternion &rotation)
{
    m_transformer.rotateLocal(rotation);
}

void lcf::Object3D::rotateLocal(float angle_deg, float x, float y, float z)
{
    m_transformer.rotateLocal(Quaternion::fromAxisAndAngle(x, y, z, angle_deg));
}

void lcf::Object3D::rotateLocalXAxis(float angle_deg)
{
    m_transformer.rotateLocalXAxis(angle_deg);
}

void lcf::Object3D::rotateLocalYAxis(float angle_deg)
{
    m_transformer.rotateLocalYAxis(angle_deg);
}

void lcf::Object3D::rotateLocalZAxis(float angle_deg)
{
    m_transformer.rotateLocalZAxis(angle_deg);
}

void lcf::Object3D::rotateAround(const Quaternion & rotation, const Vector3D & position)
{
    m_transformer.rotateAround(rotation, position);
}

void lcf::Object3D::rotateAroundOrigin(const Quaternion &rotation)
{
    m_transformer.rotateAroundOrigin(rotation);
}

void lcf::Object3D::rotateAroundSelf(const Quaternion &rotation)
{
    m_transformer.rotateAroundSelf(rotation);
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

lcf::Vector3D lcf::Object3D::getLocalPosition() const
{
    return m_transformer.getTranslation();
}

lcf::Vector3D lcf::Object3D::getWorldPosition() const
{
    return m_transformer.getWorldPosition();
}

lcf::Vector3D lcf::Object3D::translation() const
{
    return this->getLocalPosition();
}

lcf::Quaternion lcf::Object3D::rotation() const
{
    return m_transformer.getRotation();
}

lcf::Vector3D lcf::Object3D::scale() const
{
    return m_transformer.getScale();
}

const lcf::Matrix4x4 &lcf::Object3D::getWorldMatrix() const
{
    return m_transformer.getWorldMatrix();
}

const lcf::Matrix4x4 &lcf::Object3D::getInversedWorldMatrix() const
{
    return m_transformer.getInvertedHierarchialMatrix();
}

lcf::Matrix3x3 lcf::Object3D::getNormalMatrix() const
{
    return m_transformer.getNormalMatrix();
}

void lcf::Object3D::setName(std::string_view name)
{
    m_name = name;
}

const std::string &lcf::Object3D::getName() const
{
    return m_name;
}

lcf::Object3DHierarchicalTransformer &lcf::Object3D::getTransformer()
{
    return m_transformer;
}
