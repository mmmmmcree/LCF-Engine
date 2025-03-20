#pragma once

#include <QQuaternion>
#include "bullet/btBulletLinearMathCommon.h"
#include "assimp/quaternion.h"

namespace lcf {
    using Quaternion = QQuaternion;
    
    Quaternion toQuaternion(const btQuaternion &quat);
    btQuaternion toBtQuaternion(const Quaternion &quat);

    Quaternion toQuaternion(const aiQuaternion &quat);
}