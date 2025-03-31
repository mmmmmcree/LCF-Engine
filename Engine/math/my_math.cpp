#include "my_math.h"
#include <cmath>

template <>
lcf::Quaternion lcf::lerp(const lcf::Quaternion& a, const lcf::Quaternion& b, float t)
{
    return Quaternion::slerp(a, b, t);
}

float lcf::linear_interpolation_factor(float t, float a, float b)
{
    if (a == b) { return std::nanf(nullptr); }
    t = std::clamp(t, a, b);
    return (t - a) / (b - a);
}

float lcf::max(const Vector3D &vec)
{
    return std::max(std::max(vec.getX(), vec.getY()), vec.getZ());
}

lcf::Vector3D lcf::max(const Vector3D &a, const Vector3D &b)
{
    return Vector3D(std::max(a.getX(), b.getX()), std::max(a.getX(), b.getX()), std::max(a.getX(), b.getX()));
}

lcf::Vector3D lcf::max(const Vector3D &a, float value)
{
    return Vector3D(std::max(a.getX(), value), std::max(a.getY(), value), std::max(a.getZ(), value));
}

lcf::Vector3D lcf::min(const Vector3D &a, const Vector3D &b)
{
    return Vector3D(std::min(a.getX(), b.getX()), std::min(a.getY(), b.getY()), std::min(a.getZ(), b.getZ()));
}

lcf::Vector3D lcf::min(const Vector3D &a, float value)
{
    return Vector3D(std::min(a.getX(), value), std::min(a.getY(), value), std::min(a.getZ(), value));
}
