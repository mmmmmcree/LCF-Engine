#include "Matrix.h"

lcf::Matrix4x4 lcf::toMatrix4x4(const aiMatrix4x4 &at_mat)
{
    return Matrix4x4(
        at_mat.a1, at_mat.a2, at_mat.a3, at_mat.a4,
        at_mat.b1, at_mat.b2, at_mat.b3, at_mat.b4,
        at_mat.c1, at_mat.c2, at_mat.c3, at_mat.c4,
        at_mat.d1, at_mat.d2, at_mat.d3, at_mat.d4
    );
}

lcf::Vector3D lcf::toVector3D(const aiColor4D &ai_color)
{
    return Vector3D(ai_color.r, ai_color.g, ai_color.b);
}

lcf::Vector4D lcf::toVector4D(const aiColor4D &ai_color)
{
    return Vector4D(ai_color.r, ai_color.g, ai_color.b, ai_color.a);
}