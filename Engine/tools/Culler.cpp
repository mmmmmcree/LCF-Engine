#include "Culler.h"
#include "Plane.h"

bool lcf::Culler::isIntersectedWithFrustumPlanes(const Sphere &sphere)
{
    for (const auto &plane : m_frustum_planes) {
        float distance = plane.getDistanceToPoint(sphere.getCenter());
        if (distance + sphere.getRadius() < std::numeric_limits<float>::epsilon()) {
            return false;
        }
    }
    return true;
}

bool lcf::Culler::isCulled(const Sphere &sphere)
{
    return not this->isIntersectedWithFrustumPlanes(sphere);
}

void lcf::Culler::setFrustumPlanes(const Matrix4x4 &matrix)
{
    Vector4D row_3 = matrix.row(3);
    for (int i = 0; i < 3; ++i) {
        Vector4D row_i = matrix.row(i);
        m_frustum_planes[2 * i].setParameters(row_3 - row_i);
        m_frustum_planes[2 * i + 1].setParameters(row_3 + row_i);
    }
}
