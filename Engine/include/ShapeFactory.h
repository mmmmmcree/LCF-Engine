#pragma once

#include "Shapes.h"
#include <span>

namespace lcf {
    struct ShapeFactory
    {
        template <ShapeConcept S>
        static typename S::UniquePtr createUnique();

        template <ShapeConcept S>
        static typename S::UniquePtr createUnique(std::span<Vector3D> positions);
    };
}

template <lcf::ShapeConcept S>
inline typename S::UniquePtr lcf::ShapeFactory::createUnique()
{
    return std::make_unique<S>();
}

template <lcf::ShapeConcept S>
inline typename S::UniquePtr lcf::ShapeFactory::createUnique(std::span<Vector3D> positions)
{
    return std::make_unique<S>(positions);
}
