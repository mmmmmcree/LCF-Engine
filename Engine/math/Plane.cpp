#include "Plane.h"

lcf::Plane::Plane(const Vector3D &normal, float constant)
{
    this->setParameters(normal, constant);
}

lcf::Plane::Plane(float a, float b, float c, float d)
{
    this->setParameters({a, b, c}, d);
}

lcf::Plane::Plane(const Vector4D &parameters)
{
    this->setParameters(parameters);
}

void lcf::Plane::setParameters(const Vector3D &normal, float constant)
{
    m_normal = normal;
    m_constant = constant;
    float length = m_normal.length();
    if (length > std::numeric_limits<float>::epsilon()) {
        m_normal /= length;
        m_constant /= length;
    }
}

void lcf::Plane::setParameters(const Vector4D &parameters)
{
    this->setParameters(parameters.toVector3D(), parameters.getW());
}

float lcf::Plane::getDistanceToPoint(const Vector3D &point) const
{
    return Vector3D::dotProduct(m_normal, point) + m_constant;
}

const lcf::Vector3D &lcf::Plane::getNormal() const
{
    return m_normal;
}

float lcf::Plane::getConstant() const
{
    return m_constant;
}