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