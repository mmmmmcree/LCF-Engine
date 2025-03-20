#pragma once

#include <QVector4D>
#include "bullet/btBulletLinearMathCommon.h"
#include "assimp/vector3.h"

namespace lcf {
    using Vector2D = QVector2D;
    using Vector3D = QVector3D;
    using Vector4D = QVector4D;

    Vector3D toVector3D(const btVector3& vec);
    btVector3 toBtVector3(const Vector3D& vec);

    Vector3D toVector3D(const aiVector3D& vec);

    Vector4D toVector4D(const btVector4& vec);
}