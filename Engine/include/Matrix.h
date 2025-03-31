#pragma once

#include <QMatrix4x4>
#include "Vector.h"
#include "Quaternion.h"
#include "bullet/btBulletLinearMathCommon.h"
#include "assimp/matrix4x4.h"
#include "assimp/color4.h"
#include "GLMMatrix.h"

namespace lcf {
    using Matrix3x3 = GLMMatrix3x3;
    using Matrix4x4 = GLMMatrix4x4;

    Matrix4x4 toMatrix4x4(const aiMatrix4x4& at_mat);

    Vector3D toVector3D(const aiColor4D& ai_color);

    Vector4D toVector4D(const aiColor4D& ai_color);
}