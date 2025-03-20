#pragma once

#include "Shape.h"
#include "Vector.h"
#include <span>

namespace lcf {
    class Box : public ShapeMixin<ShapeType::Box>
    {
    public:
        using UniquePtr = std::unique_ptr<Box>;
        Box();
        Box(const Vector3D & min_position, const Vector3D & max_position);
        Box(std::span<Vector3D> positions);
        Box(const Box &other) = default;
        Vector3D getCenter() const;
        float getWidth() const;
        float getHeight() const;
        float getDepth() const;
        Vector3D getDimensions() const;
        friend Box operator+(const Box &lhs, const Box &rhs);
        friend Box &operator+=(Box &lhs, const Box &rhs);
    private:
        Vector3D m_min_position;
        Vector3D m_max_position;
    };
}