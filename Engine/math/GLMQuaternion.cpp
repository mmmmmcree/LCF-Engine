#include "GLMQuaternion.h"
#include "GLMVector.h"
#include "GLMMatrix.h"
#include <QQuaternion>

lcf::GLMQuaternion lcf::GLMQuaternion::fromAxisAndAngle(const GLMVector3D &axis, float angle_deg)
{
    return glm::angleAxis(glm::radians(angle_deg), glm::normalize(axis));
}

lcf::GLMQuaternion lcf::GLMQuaternion::fromAxisAndAngle(float x, float y, float z, float angle_deg)
{
    return fromAxisAndAngle(glm::vec3(x, y, z), angle_deg);
}

lcf::GLMQuaternion lcf::GLMQuaternion::slerp(const GLMQuaternion &lhs, const GLMQuaternion &rhs, float t)
{
    return glm::slerp(lhs, rhs, t);
}

lcf::GLMQuaternion::GLMQuaternion(float scalar, float x, float y, float z) :
    glm::quat(scalar, x, y, z)
{
}

lcf::GLMQuaternion::GLMQuaternion(const GLMQuaternion &quaternion) : glm::quat(quaternion)
{
}

lcf::GLMQuaternion &lcf::GLMQuaternion::operator=(const glm::quat &quaternion)
{
    memcpy(this, &quaternion, sizeof(glm::quat));
    return *this;
}

lcf::GLMQuaternion::GLMQuaternion(const glm::quat &quaternion) :
    glm::quat(quaternion)
{
}

bool lcf::GLMQuaternion::isIdentity() const
{
    return this->x == 0.0f and this->y == 0.0f and this->z == 0.0f and this->w == 1.0f;
}

float lcf::GLMQuaternion::length() const
{
    return glm::length(static_cast<glm::quat>(*this));
}

float lcf::GLMQuaternion::lengthSquared() const
{
    return this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w;
}

lcf::GLMQuaternion lcf::GLMQuaternion::conjugated() const
{
    return glm::conjugate(*this);
}

void lcf::GLMQuaternion::normalize()
{
    *this = glm::normalize(*this);
}

lcf::GLMQuaternion lcf::GLMQuaternion::normalized() const
{
    return glm::normalize(*this);
}

lcf::GLMQuaternion lcf::GLMQuaternion::inverted() const
{
    return glm::inverse(*this);
}

lcf::GLMVector3D lcf::GLMQuaternion::rotatedVector(const GLMVector3D &vector) const
{
    auto r = *this * glm::quat(0.0f, vector) * glm::conjugate(*this);
    return GLMVector3D(r.x, r.y, r.z);
}

bool lcf::GLMQuaternion::isNormalized() const
{
    return true;
}

bool lcf::GLMQuaternion::isNull() const
{
    return this->x == 0.0f and this->y == 0.0f and this->z == 0.0f;
}

void lcf::GLMQuaternion::setX(float x)
{
    this->x = x;
}

float lcf::GLMQuaternion::getX() const
{
    return this->x;
}

void lcf::GLMQuaternion::setY(float y)
{
    this->y = y;
}

float lcf::GLMQuaternion::getY() const
{
    return this->y;
}

void lcf::GLMQuaternion::setZ(float z)
{
    this->z = z;
}

float lcf::GLMQuaternion::getZ() const
{
    return this->z;
}

void lcf::GLMQuaternion::setScalar(float w)
{
    this->w = w;
}

float lcf::GLMQuaternion::getScalar() const
{
    return this->w;
}

lcf::GLMVector3D lcf::GLMQuaternion::toVector3D() const
{
    return GLMVector3D(this->x, this->y, this->z);
}

lcf::GLMVector4D lcf::GLMQuaternion::toVector4D() const
{
    return GLMVector4D(this->x, this->y, this->z, this->w);
}

lcf::GLMMatrix3x3 lcf::GLMQuaternion::toMatrix3x3() const
{
    return glm::mat3_cast(*this);
}

lcf::GLMMatrix4x4 lcf::GLMQuaternion::toMatrix4x4() const
{
    return glm::mat4_cast(*this);
}

void lcf::GLMQuaternion::getAxisAndAngle(float *x, float *y, float *z, float *angle_deg)
{
    QQuaternion q(this->getScalar(), this->getX(), this->getY(), this->getZ());
    q.getAxisAndAngle(x, y, z, angle_deg);
}

QDebug lcf::operator<<(QDebug debug, const GLMQuaternion &quaternion)
{
    debug.nospace() << "GLMQuaternion(" << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << ", " << quaternion.w << ")";
    return debug.space();
}
