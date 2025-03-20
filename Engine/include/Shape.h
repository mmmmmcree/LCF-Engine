#pragma once

#include <memory>
#include "TypeConsistencyChecker.h"

namespace lcf {
    enum class ShapeType
    {
        Box,
        Sphere,
    };

    class Shape : public TypeConsistencyChecker<ShapeType>
    {
    public:
        using EnumType = ShapeType;
        using UniquePtr = std::unique_ptr<Shape>;
        Shape() = default;
        virtual ~Shape() = default;
    };

    template <typename T>
    concept ShapeConcept = std::is_base_of_v<Shape, T>;

    template <ShapeType EnumValue>
    using ShapeMixin = TypeConsistencyCheckerMixin<ShapeType, EnumValue, Shape>;
}