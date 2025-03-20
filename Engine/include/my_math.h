#pragma once

#include "Matrix.h"

namespace lcf {
    template <typename T>
    T lerp(const T &a, const T &b, float t);

    template <>
    Quaternion lerp(const Quaternion &a, const Quaternion &b, float t);

    float linear_interpolation_factor(float t, float a, float b);

    float max(const Vector3D &vec);

    Vector3D max(const Vector3D &a, const Vector3D &b);

    Vector3D max(const Vector3D &a, float value);

    Vector3D min(const Vector3D &a, const Vector3D &b);

    Vector3D min(const Vector3D &a, float value);
}

template <typename T>
T lcf::lerp(const T &a, const T &b, float t)
{
    return a + (b - a) * t;
}