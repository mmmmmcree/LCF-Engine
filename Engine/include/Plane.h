#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace lcf {
    class Plane
    {
    public:
        Plane() = default;
        Plane(const Vector3D& normal, float constant);
        Plane(float a, float b, float c, float d);
        Plane(const Vector4D &parameters);
        Plane(const Plane &other) = default;
        void setParameters(const Vector3D& normal, float constant);
        void setParameters(const Vector4D &parameters);
        float getDistanceToPoint(const Vector3D& point) const;
        const Vector3D & getNormal() const;
        float getConstant() const;
    private:
        Vector3D m_normal;
        float m_constant;
    };
}