#include "Sphere.h"
#include "my_math.h"

lcf::Sphere::Sphere(const Vector3D &center, float radius) :
    BaseConstructor(),
    m_center(center),
    m_radius(radius)
{
}

lcf::Sphere::Sphere(std::span<Vector3D> positions) :
    BaseConstructor()
{
    for (const auto &position : positions) {
        m_center += position;
    }
    m_center /= positions.size();
    float max_distance_squared = 0.0f;
    for (size_t i = 0; i < positions.size(); ++i) {
        Vector3D diff = positions[i] - m_center;
        max_distance_squared = std::max(max_distance_squared, Vector3D::dotProduct(diff, diff));
    }
    m_radius = std::sqrt(max_distance_squared);
}

void lcf::Sphere::setRadius(float radius)
{
    m_radius = radius;
}

float lcf::Sphere::getRadius() const
{
    return m_radius;
}

void lcf::Sphere::setCenter(const Vector3D &center)
{
    m_center = center;
}

const lcf::Vector3D &lcf::Sphere::getCenter() const
{
    return m_center;
}

void lcf::Sphere::transform(const Vector3D &translation, const Vector3D &scale)
{
    m_center *= scale;
    m_center += translation;
    m_radius *= max(scale);
}