#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLMVector.h"
#include "GLMQuaternion.h"
#include <QDebug>

namespace lcf {
    using GLMMatrix3x3 = glm::mat3;
    class GLMMatrix4x4 : public glm::mat4
    {
        friend QDebug operator<<(QDebug debug, const GLMMatrix4x4 &mat);
    public:
        GLMMatrix4x4();
        GLMMatrix4x4(const GLMMatrix4x4 &mat) = default;
        GLMMatrix4x4(GLMMatrix4x4 &&mat) = default;
        GLMMatrix4x4(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
        GLMMatrix4x4 &operator=(const GLMMatrix4x4 &mat);
        GLMMatrix4x4 &operator=(const glm::mat4 &mat);
        GLMMatrix4x4(const glm::mat4 &mat);
        GLMMatrix4x4(glm::mat4 &&mat);
        GLMMatrix4x4 operator*(const GLMMatrix4x4 &mat) const;
        GLMMatrix4x4 &operator*=(const GLMMatrix4x4 &mat);
        void setToIdentity();
        void setTranslation(const GLMVector3D &translation);
        void setTranslation(float x, float y, float z);
        GLMVector3D getTranslation() const;
        void setRotation(const GLMQuaternion &rotation);
        void setRotation(float angle_deg, const GLMVector3D &axis);
        void setRotation(float angle_deg, float x, float y, float z);
        GLMQuaternion getRotation() const;
        void setScale(const GLMVector3D &scale);
        void setScale(float x, float y, float z);
        GLMVector3D getScale() const;
        void setTRS(const GLMVector3D &translation, const GLMQuaternion &rotation, const GLMVector3D &scale);
        void translateLocal(const GLMVector3D &translation); // 右乘变换矩阵 
        void translateLocal(float x, float y, float z);
        void translateLocalX(float x);
        void translateLocalY(float y);
        void translateLocalZ(float z);
        void translateWorld(const GLMVector3D &translation); // 左乘变换矩阵
        void translateWorld(float x, float y, float z);
        void rotateLocal(const GLMQuaternion &rotation); // 右乘变换矩阵
        void rotateLocal(float angle_deg, const GLMVector3D &axis);
        void rotateLocal(float angle_deg, float x, float y, float z);
        void rotateAround(const GLMQuaternion &rotation, const GLMVector3D &position); // 左乘变换矩阵
        void rotateAroundOrigin(const GLMQuaternion&rotation);
        void rotateAroundSelf(const GLMQuaternion &rotation);
        void scale(const GLMVector3D &scale); // 右乘变换矩阵
        void scale(float x, float y, float z);
        void scale(float factor);
        void inverse();
        GLMMatrix4x4 inverted() const;
        void transpose();
        GLMMatrix4x4 transposed() const;
        void lookAt(const GLMVector3D &eye, const GLMVector3D &center, const GLMVector3D &up);
        void ortho(float left, float right, float bottom, float top, float near_plane, float far_plane);
        void perspective(float fovy_deg, float aspect, float near_plane, float far_plane);
        float *data();
        const float *data() const;
        const float *constData() const;
        GLMMatrix3x3 toMatrix3x3() const;
        void setColumn(int index, const GLMVector4D &column);
        GLMVector4D column(int index) const;
        void setRow(int index, const GLMVector4D &row);
        GLMVector4D row(int index) const;
    };
}