#include "Quaternion.h"

lcf::Quaternion lcf::toQuaternion(const btQuaternion &quat)
{
    return Quaternion(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
}

btQuaternion lcf::toBtQuaternion(const Quaternion &quat)
{
    return btQuaternion(quat.x(), quat.y(), quat.z(), quat.scalar());
}

lcf::Quaternion lcf::toQuaternion(const aiQuaternion &ai_quat)
{
    return Quaternion(ai_quat.w, ai_quat.x, ai_quat.y, ai_quat.z);
}
