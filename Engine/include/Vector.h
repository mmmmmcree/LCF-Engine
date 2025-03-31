#pragma once

#include <QVector4D>
#include "bullet/btBulletLinearMathCommon.h"
#include "assimp/vector3.h"
#include "GLMVector.h"

namespace lcf {
    using Vector2D = QVector2D;
    using Vector3D = GLMVector3D;
    using Vector4D = GLMVector4D;

    Vector3D toVector3D(const btVector3& vec);
    btVector3 toBtVector3(const Vector3D& vec);

    Vector3D toVector3D(const aiVector3D& vec);

    Vector4D toVector4D(const btVector4& vec);
}