#include "Vector.h"

lcf::Vector3D lcf::toVector3D(const btVector3 &vec)
{
    return Vector3D(vec.getX(), vec.getY(), vec.getZ());
}

btVector3 lcf::toBtVector3(const Vector3D &vec)
{
    return btVector3(vec.getX(), vec.getY(), vec.getZ());
}

lcf::Vector3D lcf::toVector3D(const aiVector3D & ai_vec)
{
    return Vector3D(ai_vec.x, ai_vec.y, ai_vec.z);
}

lcf::Vector4D lcf::toVector4D(const btVector4 &vec)
{
    return Vector4D(vec.getX(), vec.getY(), vec.getZ(), vec.getW());
}
