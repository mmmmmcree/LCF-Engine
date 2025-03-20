#pragma once

#include "Shapes.h"
#include "Matrix.h"
#include "Plane.h"
#include <array>

namespace lcf {
    class Culler
    {
    public:
        using PlaneArray = std::array<Plane, 6>;
        Culler() = default;
        virtual bool isCulled(const Sphere &sphere);
        void setFrustumPlanes(const Matrix4x4 &matrix);
        bool isIntersectedWithFrustumPlanes(const Sphere &sphere);
    private:
        PlaneArray m_frustum_planes;
    };
}