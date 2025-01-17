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

lcf::Vector3D lcf::toVector3D(const aiVector3D & ai_vec)
{
    return Vector3D(ai_vec.x, ai_vec.y, ai_vec.z);
}

lcf::Vector3D lcf::toVector3D(const aiColor4D &ai_color)
{
    return Vector3D(ai_color.r, ai_color.g, ai_color.b);
}

lcf::Vector4D lcf::toVector4D(const aiColor4D &ai_color)
{
    return Vector4D(ai_color.r, ai_color.g, ai_color.b, ai_color.a);
}

lcf::Quaternion lcf::toQuaternion(const aiQuaternion &ai_quat)
{
    return Quaternion(ai_quat.w, ai_quat.x, ai_quat.y, ai_quat.z);
    
}

lcf::DecomposedTransform lcf::decompose(const Matrix4x4 &mat)
{
    Vector3D translation = mat.column(3).toVector3D();
    Vector3D scale;
    for (int i = 0; i < 3; ++i) {
        scale[i] = mat.column(i).toVector3D().length();
    }
    Matrix4x4 rotation_mat = mat;
    for (int i = 0; i < 3; ++i) {
        if (scale[i] == 0.0f) { continue; }
        rotation_mat.setColumn(i, mat.column(i) / scale[i]);
    }
    Quaternion rotation = Quaternion::fromRotationMatrix(rotation_mat.toGenericMatrix<3, 3>());
    return DecomposedTransform(std::move(translation), std::move(scale), std::move(rotation));
}

lcf::DecomposedTransform::DecomposedTransform(const Vector3D &t, const Vector3D &s, const Quaternion &r) :
    translation(t), scale(s), rotation(r)
{
}

lcf::DecomposedTransform::DecomposedTransform(Vector3D &&t, Vector3D &&s, Quaternion &&r) :
    translation(std::move(t)), scale(std::move(s)), rotation(std::move(r))
{
}

lcf::DecomposedTransform::DecomposedTransform(const DecomposedTransform &other) :
    translation(other.translation), scale(other.scale), rotation(other.rotation)
{
}

lcf::DecomposedTransform &lcf::DecomposedTransform::operator=(DecomposedTransform &&other)
{
    translation = std::move(other.translation);
    scale = std::move(other.scale);
    rotation = std::move(other.rotation);
    return *this;
}

lcf::Matrix4x4 lcf::DecomposedTransform::toTransform() const
{
    Matrix4x4 translation_mat, scale_mat, rotation_mat;
    translation_mat.translate(translation);
    scale_mat.scale(scale);
    rotation_mat.rotate(rotation);
    return translation_mat * rotation_mat * scale_mat;
}
