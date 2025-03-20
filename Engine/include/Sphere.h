#pragma once

#include "Shape.h"
#include "Vector.h"
#include "Matrix.h"
#include <span>

namespace lcf {
    class Sphere : public ShapeMixin<ShapeType::Sphere>
    {
    public:
        using UniquePtr = std::unique_ptr<Sphere>;
        Sphere() = default;
        Sphere(const Vector3D& center, float radius);
        Sphere(std::span<Vector3D> positions);
        Sphere(const Sphere &other) = default;
        void setRadius(float radius);
        float getRadius() const;
        void setCenter(const Vector3D & center);
        const Vector3D & getCenter() const;
        void transform(const Vector3D &translation, const Vector3D &scale);
    private:
        Vector3D m_center;
        float m_radius = 0.0f;
    };
}