#include "GLMMatrix.h"
#include <glm/gtc/type_ptr.hpp>

lcf::GLMMatrix4x4::GLMMatrix4x4() :
    glm::mat4(1.0f)
{
}

lcf::GLMMatrix4x4::GLMMatrix4x4(const glm::mat4 &mat) :
    glm::mat4(mat)
{
}

lcf::GLMMatrix4x4::GLMMatrix4x4(glm::mat4 &&mat) :
    glm::mat4(std::move(mat))
{
}

lcf::GLMMatrix4x4 lcf::GLMMatrix4x4::operator*(const GLMMatrix4x4 &mat) const
{
    return static_cast<glm::mat4>(*this) * static_cast<glm::mat4>(mat);
}

lcf::GLMMatrix4x4 &lcf::GLMMatrix4x4::operator*=(const GLMMatrix4x4 &mat)
{
    return *this = *this * mat;
}

lcf::GLMMatrix4x4::GLMMatrix4x4(
    float m11, float m12, float m13, float m14,
    float m21, float m22, float m23, float m24,
    float m31, float m32, float m33, float m34,
    float m41, float m42, float m43, float m44)
{
    this->operator[](0) = GLMVector4D(m11, m21, m31, m41);
    this->operator[](1) = GLMVector4D(m12, m22, m32, m42);
    this->operator[](2) = GLMVector4D(m13, m23, m33, m43);
    this->operator[](3) = GLMVector4D(m14, m24, m34, m44);
}

lcf::GLMMatrix4x4 &lcf::GLMMatrix4x4::operator=(const GLMMatrix4x4 &mat)
{
    memcpy(this, &mat, sizeof(GLMMatrix4x4));
    return *this;
}

lcf::GLMMatrix4x4 &lcf::GLMMatrix4x4::operator=(const glm::mat4 &mat)
{
    memcpy(this, &mat, sizeof(glm::mat4));
    return *this;
}

void lcf::GLMMatrix4x4::setToIdentity()
{
    *this = glm::identity<glm::mat4>();
}

void lcf::GLMMatrix4x4::setTranslation(const GLMVector3D &translation)
{
    memcpy(&this->operator[](3), &translation, sizeof(GLMVector3D));
}

void lcf::GLMMatrix4x4::setTranslation(float x, float y, float z)
{
    return this->setTranslation({x, y, z});
}

lcf::GLMVector3D lcf::GLMMatrix4x4::getTranslation() const
{
    return glm::vec3(this->operator[](3));
}

void lcf::GLMMatrix4x4::setRotation(const GLMQuaternion&rotation)
{
    auto rotation_mat = rotation.toMatrix3x3();
    memcpy(&this->operator[](0), &rotation_mat[0], sizeof(glm::vec3));
    memcpy(&this->operator[](1), &rotation_mat[1], sizeof(glm::vec3));
    memcpy(&this->operator[](2), &rotation_mat[2], sizeof(glm::vec3));
}

void lcf::GLMMatrix4x4::setRotation(float angle_deg, const GLMVector3D &axis)
{
    this->setRotation(GLMQuaternion::fromAxisAndAngle(axis, angle_deg));
}

void lcf::GLMMatrix4x4::setRotation(float angle_deg, float x, float y, float z)
{
    this->setRotation(angle_deg, {x, y, z});
}

lcf::GLMQuaternion lcf::GLMMatrix4x4::getRotation() const
{
    GLMMatrix3x3 mat = *this;
    for (int i = 0; i < 3; ++i) {
        mat[i] /= glm::length(mat[i]);
    }
    return glm::quat_cast(mat);
}

void lcf::GLMMatrix4x4::setScale(const GLMVector3D &scale)
{
    auto s = glm::max(scale, std::numeric_limits<float>::epsilon());
    glm::vec3 original_scale(glm::length(this->operator[](0)), glm::length(this->operator[](1)), glm::length(this->operator[](2)));
    this->operator[](0) *= s.x / original_scale.x;
    this->operator[](1) *= s.y / original_scale.y;
    this->operator[](2) *= s.z / original_scale.z;
}

void lcf::GLMMatrix4x4::setScale(float x, float y, float z)
{
    this->setScale(glm::vec3(x, y, z));
}

lcf::GLMVector3D lcf::GLMMatrix4x4::getScale() const
{
    return glm::vec3(glm::length(this->operator[](0)), glm::length(this->operator[](1)), glm::length(this->operator[](2)));
}

void lcf::GLMMatrix4x4::setTRS(const GLMVector3D &translation, const GLMQuaternion &rotation, const GLMVector3D &scale)
{
    this->setTranslation(translation);
    this->setRotation(rotation);
    this->setScale(scale);
}

void lcf::GLMMatrix4x4::translateLocal(const GLMVector3D &translation)
{
    *this = glm::translate(*this, translation);
}

void lcf::GLMMatrix4x4::translateLocal(float x, float y, float z)
{
    this->translateLocal({x, y, z});
}

void lcf::GLMMatrix4x4::translateLocalX(float x)
{
    this->translateLocal({x, 0.0f, 0.0f});
}

void lcf::GLMMatrix4x4::translateLocalY(float y)
{
    this->translateLocal({0.0f, y, 0.0f});
}

void lcf::GLMMatrix4x4::translateLocalZ(float z)
{
    this->translateLocal({0.0f, 0.0f, z});
}

void lcf::GLMMatrix4x4::translateWorld(const GLMVector3D &translation)
{
    this->operator[](3) += translation.toVector4D();
}

void lcf::GLMMatrix4x4::translateWorld(float x, float y, float z)
{
    this->translateWorld(glm::vec3(x, y, z));
}

void lcf::GLMMatrix4x4::rotateLocal(const GLMQuaternion &rotation)
{
    this->operator*=(static_cast<glm::mat4>(rotation.toMatrix4x4()));
}

void lcf::GLMMatrix4x4::rotateLocal(float angle_deg, const GLMVector3D &axis)
{
    *this = glm::rotate(*this, glm::radians(angle_deg), axis);
}

void lcf::GLMMatrix4x4::rotateLocal(float angle_deg, float x, float y, float z)
{
    this->rotateLocal(angle_deg, {x, y, z});
}

void lcf::GLMMatrix4x4::rotateAround(const GLMQuaternion &rotation, const GLMVector3D &position)
{
    GLMMatrix4x4 mat = rotation.toMatrix4x4();
    GLMVector3D translation = position + rotation.rotatedVector(-position);
    memcpy(&mat[3], &translation, sizeof(GLMVector3D));
    *this = mat * (*this);
}

void lcf::GLMMatrix4x4::rotateAroundOrigin(const GLMQuaternion &rotation)
{
    *this = rotation.toMatrix4x4() * (*this);
}

void lcf::GLMMatrix4x4::rotateAroundSelf(const GLMQuaternion &rotation)
{
    this->rotateAround(rotation, this->getTranslation());
}

void lcf::GLMMatrix4x4::scale(const GLMVector3D &scale)
{
    *this = glm::scale(*this, scale);
}

void lcf::GLMMatrix4x4::scale(float x, float y, float z)
{
    this->scale(glm::vec3(x, y, z));
}

void lcf::GLMMatrix4x4::scale(float factor)
{
    this->scale(glm::vec3(factor));
}

void lcf::GLMMatrix4x4::inverse()
{
    *this = glm::inverse(*this);
}

float *lcf::GLMMatrix4x4::data()
{
    return glm::value_ptr(static_cast<glm::mat4&>(*this));
}
const float *lcf::GLMMatrix4x4::data() const
{
    return glm::value_ptr(static_cast<const glm::mat4&>(*this));
}

const float *lcf::GLMMatrix4x4::constData() const
{
    return glm::value_ptr(static_cast<const glm::mat4&>(*this));
}

lcf::GLMMatrix4x4 lcf::GLMMatrix4x4::inverted() const
{
    return glm::inverse(*this);
}

void lcf::GLMMatrix4x4::transpose()
{
    *this = glm::transpose(*this);
}

lcf::GLMMatrix4x4 lcf::GLMMatrix4x4::transposed() const
{
    return glm::transpose(*this);
}

void lcf::GLMMatrix4x4::lookAt(const GLMVector3D &eye, const GLMVector3D &center, const GLMVector3D &up)
{
    *this = glm::lookAt(eye, center, up);
}

void lcf::GLMMatrix4x4::ortho(float left, float right, float bottom, float top, float near_plane, float far_plane)
{
    *this = glm::ortho(left, right, bottom, top, near_plane, far_plane);
}

void lcf::GLMMatrix4x4::perspective(float fovy_deg, float aspect, float near_plane, float far_plane)
{
    *this = glm::perspective(glm::radians(fovy_deg), aspect, near_plane, far_plane);
}

lcf::GLMMatrix3x3 lcf::GLMMatrix4x4::toMatrix3x3() const
{
    return GLMMatrix3x3(*this);
}

void lcf::GLMMatrix4x4::setColumn(int index, const GLMVector4D &column)
{
    this->operator[](index) = column;
}

lcf::GLMVector4D lcf::GLMMatrix4x4::column(int index) const
{
    return this->operator[](index);
}

void lcf::GLMMatrix4x4::setRow(int index, const GLMVector4D &row)
{
    this->operator[](0)[index] = row.x;
    this->operator[](1)[index] = row.y;
    this->operator[](2)[index] = row.z;
    this->operator[](3)[index] = row.w;
}

lcf::GLMVector4D lcf::GLMMatrix4x4::row(int index) const
{
    return GLMVector4D(this->operator[](0)[index], this->operator[](1)[index], this->operator[](2)[index], this->operator[](3)[index]);
}

QDebug lcf::operator<<(QDebug debug, const GLMMatrix4x4 &mat)
{
    debug.nospace() << "GLMTransformMatrix(type:" << '\n'
        << qSetFieldWidth(10)
        << mat[0][0] << mat[1][0] << mat[2][0] << mat[3][0] << '\n'
        << mat[0][1] << mat[1][1] << mat[2][1] << mat[3][1] << '\n'
        << mat[0][2] << mat[1][2] << mat[2][2] << mat[3][2] << '\n'
        << mat[0][3] << mat[1][3] << mat[2][3] << mat[3][3] << '\n'
        << qSetFieldWidth(0) << ')';
    return debug.space();
}