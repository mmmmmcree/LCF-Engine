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
    return std::max(std::max(vec.x(), vec.y()), vec.z());
}

lcf::Vector3D lcf::max(const Vector3D &a, const Vector3D &b)
{
    return Vector3D(std::max(a.x(), b.x()), std::max(a.y(), b.y()), std::max(a.z(), b.z()));
}

lcf::Vector3D lcf::max(const Vector3D &a, float value)
{
    return Vector3D(std::max(a.x(), value), std::max(a.y(), value), std::max(a.z(), value));
}

lcf::Vector3D lcf::min(const Vector3D &a, const Vector3D &b)
{
    return Vector3D(std::min(a.x(), b.x()), std::min(a.y(), b.y()), std::min(a.z(), b.z()));
}

lcf::Vector3D lcf::min(const Vector3D &a, float value)
{
    return Vector3D(std::min(a.x(), value), std::min(a.y(), value), std::min(a.z(), value));
}
