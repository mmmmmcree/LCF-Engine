#pragma once

#include <QMatrix4x4>
#include <QQuaternion>
#include "assimp/matrix4x4.h"
#include "assimp/vector3.h"
#include "assimp/quaternion.h"
#include "assimp/color4.h"

namespace lcf {
    using Matrix2x2 = QMatrix2x2;
    using Matrix3x3 = QMatrix3x3;
    using Matrix4x4 = QMatrix4x4;
    using Vector2D = QVector2D;
    using Vector3D = QVector3D;
    using Vector4D = QVector4D;
    using Quaternion = QQuaternion;

    Matrix4x4 toMatrix4x4(const aiMatrix4x4& at_mat);

    Vector3D toVector3D(const aiVector3D& ai_vec);

    Vector3D toVector3D(const aiColor4D& ai_color);

    Vector4D toVector4D(const aiColor4D& ai_color);

    Quaternion toQuaternion(const aiQuaternion& ai_quat);

    struct DecomposedTransform
    {
        DecomposedTransform() = default;
        DecomposedTransform(const Vector3D &t, const Vector3D &s, const Quaternion &r);
        DecomposedTransform(Vector3D &&t, Vector3D &&s, Quaternion &&r);
        DecomposedTransform(const DecomposedTransform &other);
        DecomposedTransform &operator=(const DecomposedTransform &other) = default;
        DecomposedTransform &operator=(DecomposedTransform &&other);
        Matrix4x4 toTransform() const;
        Vector3D translation;
        Vector3D scale = Vector3D(1.0, 1.0, 1.0);
        Quaternion rotation;
    };

   DecomposedTransform decompose(const Matrix4x4& mat); 
}