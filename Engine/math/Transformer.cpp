#include "Transformer.h"

lcf::Transformer::Transformer(const Transformer &other) :
    m_need_update(true),
    m_position(other.m_position),
    m_rotation(other.m_rotation),
    m_scale(other.m_scale)
{
}

lcf::Transformer &lcf::Transformer::operator=(const Transformer &other)
{
    m_need_update = true;
    m_position = other.m_position;
    m_rotation = other.m_rotation;
    m_scale = other.m_scale;
    return *this;
}

void lcf::Transformer::setMatrix(const Matrix4x4 &matrix)
{
    auto decomposed = decompose(matrix);
    m_position = decomposed.translation;
    m_rotation = decomposed.rotation;
    m_scale = decomposed.scale;
    m_matrix = matrix;
}

const lcf::Matrix4x4 &lcf::Transformer::getMatrix()
{
    if (not m_need_update) { return m_matrix; }
    m_need_update = false;
    m_matrix.setToIdentity();
    m_matrix.translate(m_position);
    m_matrix.rotate(m_rotation);
    m_matrix.scale(m_scale);
    return m_matrix;
}

void lcf::Transformer::translate(float x, float y, float z)
{
    this->translate(Vector3D(x, y, z));
}

void lcf::Transformer::translate(const Vector3D &translation)
{
    m_position += m_rotation.rotatedVector(translation) * m_scale;
    this->requireUpdate();
}

void lcf::Transformer::rotate(float angle_degrees, float x, float y, float z)
{
    this->rotate(angle_degrees, Vector3D(x, y, z));
}

void lcf::Transformer::rotate(float angle_degrees, const Vector3D &axis)
{
    this->rotate(Quaternion::fromAxisAndAngle(axis, angle_degrees));
}

void lcf::Transformer::rotate(const Quaternion &rotation)
{
    m_rotation = rotation * m_rotation;
    m_rotation.normalize();
    this->requireUpdate();
}

void lcf::Transformer::rotateXAxis(float angle_degrees)
{
    this->rotate(angle_degrees, this->getXAxis());
}

void lcf::Transformer::rotateYAxis(float angle_degrees)
{
    this->rotate(angle_degrees, this->getYAxis());
}

void lcf::Transformer::rotateZAxis(float angle_degrees)
{
    this->rotate(angle_degrees, this->getZAxis());
}

void lcf::Transformer::scale(float factor)
{
    this->scale(Vector3D(factor, factor, factor));
}

void lcf::Transformer::scale(float x, float y, float z)
{
    this->scale(Vector3D(x, y, z));
}

void lcf::Transformer::scale(const Vector3D &scale)
{
    m_scale *= scale;
    m_scale.setX(std::max(m_scale.x(), 0.0001f));
    m_scale.setY(std::max(m_scale.y(), 0.0001f));
    m_scale.setZ(std::max(m_scale.z(), 0.0001f));
    this->requireUpdate();
}

void lcf::Transformer::setPosition(float x, float y, float z)
{
    this->setPosition(Vector3D(x, y, z));
}

void lcf::Transformer::setPosition(const Vector3D &position)
{
    m_position = position;
    this->requireUpdate();
}

void lcf::Transformer::setRotation(float angle_degrees, float x, float y, float z)
{
    this->setRotation(angle_degrees, Vector3D(x, y, z));
}

void lcf::Transformer::setRotation(float angle_degrees, const Vector3D &axis)
{
    this->setRotation(Quaternion::fromAxisAndAngle(axis, angle_degrees));
}

void lcf::Transformer::setRotation(const Quaternion &rotation)
{
    m_rotation = rotation;
    this->requireUpdate();
}

void lcf::Transformer::setRotationXAxis(float angle_degrees)
{
    this->setRotation(angle_degrees, this->getXAxis());
}

void lcf::Transformer::setRotationYAxis(float angle_degrees)
{
    this->setRotation(angle_degrees, this->getYAxis());
}

void lcf::Transformer::setRotationZAxis(float angle_degrees)
{
    this->setRotation(angle_degrees, this->getZAxis());
}

void lcf::Transformer::setScale(float x, float y, float z)
{
    this->setScale(Vector3D(x, y, z));
}

void lcf::Transformer::setScale(const Vector3D &scale)
{
    m_scale = scale;
    this->requireUpdate();
}

void lcf::Transformer::setScale(float factor)
{
    this->setScale(Vector3D(factor, factor, factor));
}

lcf::Vector3D lcf::Transformer::getXAxis()
{
    return this->getMatrix().column(0).toVector3D();
}

lcf::Vector3D lcf::Transformer::getYAxis()
{
    return this->getMatrix().column(1).toVector3D();
}

lcf::Vector3D lcf::Transformer::getZAxis()
{
    return this->getMatrix().column(2).toVector3D();
}

const lcf::Vector3D &lcf::Transformer::getPosition() const
{
    return m_position;
}

const lcf::Quaternion &lcf::Transformer::getRotation() const
{
    return m_rotation;
}

const lcf::Vector3D &lcf::Transformer::getScale() const
{
    return m_scale;
}

void lcf::Transformer::requireUpdate()
{
    m_need_update = true;
}

bool lcf::Transformer::isUpdated() const
{
    return not m_need_update;
}
