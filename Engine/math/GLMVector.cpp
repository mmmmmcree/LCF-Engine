#include "GLMVector.h"

lcf::GLMVector3D lcf::GLMVector3D::crossProduct(const GLMVector3D &lhs, const GLMVector3D &rhs)
{
    return glm::cross(lhs, rhs);
}

float lcf::GLMVector3D::dotProduct(const GLMVector3D &lhs, const GLMVector3D &rhs)
{
    return glm::dot(static_cast<glm::vec3>(lhs), static_cast<glm::vec3>(rhs));
}

lcf::GLMVector3D::GLMVector3D(float x, float y, float z) : glm::vec3(x, y, z)
{
}

lcf::GLMVector3D::GLMVector3D(const glm::vec3 &vec) : glm::vec3(vec)
{
}

lcf::GLMVector3D &lcf::GLMVector3D::operator=(const glm::vec3 &vec)
{
    memcpy(this, &vec, sizeof(glm::vec3));
    return *this;
}

lcf::GLMVector3D::GLMVector3D(const GLMVector4D &vec)
{
    memcpy(this, &vec, sizeof(glm::vec3));
}

lcf::GLMVector3D lcf::GLMVector3D::operator+(const GLMVector3D &vec) const
{
    return static_cast<glm::vec3>(*this) + static_cast<glm::vec3>(vec);
}

lcf::GLMVector3D lcf::GLMVector3D::operator-(const GLMVector3D &vec) const
{
    return static_cast<glm::vec3>(*this) - static_cast<glm::vec3>(vec);
}

lcf::GLMVector3D lcf::GLMVector3D::operator*(const GLMVector3D & vec) const
{
    return static_cast<glm::vec3>(*this) * static_cast<glm::vec3>(vec);
}

lcf::GLMVector3D lcf::GLMVector3D::operator/(const GLMVector3D &vec) const
{
    return static_cast<glm::vec3>(*this) / static_cast<glm::vec3>(vec);
}

lcf::GLMVector3D lcf::GLMVector3D::operator*(float scalar) const
{
    return static_cast<glm::vec3>(*this) * scalar;
}

lcf::GLMVector3D lcf::GLMVector3D::operator/(float scalar) const
{
    return static_cast<glm::vec3>(*this) / scalar;
}

lcf::GLMVector3D &lcf::GLMVector3D::operator+=(const GLMVector3D &vec)
{
    return *this = *this + vec;
}

lcf::GLMVector3D &lcf::GLMVector3D::operator-=(const GLMVector3D &vec)
{
    return *this = *this - vec;
}

lcf::GLMVector3D &lcf::GLMVector3D::operator*=(const GLMVector3D &vec)
{
    return *this = *this * vec;
}

lcf::GLMVector3D &lcf::GLMVector3D::operator/=(const GLMVector3D &vec)
{
    return *this = *this / vec;
}

lcf::GLMVector3D &lcf::GLMVector3D::operator*=(float scalar)
{
    return *this = *this * scalar;
}

lcf::GLMVector3D &lcf::GLMVector3D::operator/=(float scalar)
{
    return *this = *this / scalar;
}

void lcf::GLMVector3D::setX(float x)
{
    this->x = x;
}

float lcf::GLMVector3D::getX() const
{
    return this->x;
}

void lcf::GLMVector3D::setY(float y)
{
    this->y = y;
}

float lcf::GLMVector3D::getY() const
{
    return this->y;
}

void lcf::GLMVector3D::setZ(float z)
{
    this->z = z;
}

float lcf::GLMVector3D::getZ() const
{
    return this->z;
}

lcf::GLMVector4D lcf::GLMVector3D::toVector4D() const
{
    return GLMVector4D(this->x, this->y, this->z, 0.0f);
}

void lcf::GLMVector3D::normalize()
{
    if (this->isNull()) { return; }
    *this = glm::normalize(*this);
}

lcf::GLMVector3D lcf::GLMVector3D::normalized() const
{
    if (this->isNull()) { return { 0.0f, 0.0f, 0.0f }; }
    return glm::normalize(*this);
}

bool lcf::GLMVector3D::isNull() const
{
    return this->x == 0.0f and this->y == 0.0f and this->z == 0.0f;
}

lcf::GLMVector4D::GLMVector4D(float x, float y, float z, float w) :
    glm::vec4(x, y, z, w)
{
}

lcf::GLMVector4D::GLMVector4D(const GLMVector3D &vec, float w) :
    glm::vec4(vec, w)
{
}

lcf::GLMVector4D::GLMVector4D(const glm::vec4 &vec) : glm::vec4(vec)
{
}

lcf::GLMVector4D &lcf::GLMVector4D::operator=(const glm::vec4 &vec)
{
    memcpy(this, &vec, sizeof(glm::vec4));    
    return *this;
}

lcf::GLMVector4D::GLMVector4D(const GLMVector3D &vec) :
    glm::vec4(vec, 0.0f)
{
}

void lcf::GLMVector4D::setX(float x)
{
    this->x = x;
}

float lcf::GLMVector4D::getX() const
{
    return this->x;
}

void lcf::GLMVector4D::setY(float y)
{
    this->y = y;
}

float lcf::GLMVector4D::getY() const
{
    return this->y;
}

void lcf::GLMVector4D::setZ(float z)
{
    this->z = z;
}

float lcf::GLMVector4D::getZ() const
{
    return this->z;
}

void lcf::GLMVector4D::setW(float w)
{
    this->w = w;
}

float lcf::GLMVector4D::getW() const
{
    return this->w;
}

lcf::GLMVector3D lcf::GLMVector4D::toVector3D() const
{
    return GLMVector3D(this->x, this->y, this->z);
}

void lcf::GLMVector4D::normalize()
{
    *this = glm::normalize(*this);
}

lcf::GLMVector4D lcf::GLMVector4D::normalized() const
{
    return glm::normalize(*this);
}

bool lcf::GLMVector4D::isNull() const
{
    return this->x == 0.0f and this->y == 0.0f and this->z == 0.0f and this->w == 0.0f;
}

QDebug lcf::operator<<(QDebug dbg, const GLMVector3D &vec)
{
    dbg.nospace() << "GLMVector3D(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << ")";
    return dbg.space();
}

QDebug lcf::operator<<(QDebug dbg, const GLMVector4D &vec)
{
    dbg.nospace() << "GLMVector4D(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ", " << ")";
    return dbg.space();
}
