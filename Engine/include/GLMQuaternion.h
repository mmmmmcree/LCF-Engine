#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <QDebug>

namespace lcf {
    class GLMVector3D;
    class GLMVector4D;
    // class GLMMatrix3x3;
    using GLMMatrix3x3 = glm::mat3;
    class GLMMatrix4x4;
    class GLMQuaternion : public glm::quat
    {
        friend QDebug operator<<(QDebug debug, const GLMQuaternion &quaternion);
    public:
        static GLMQuaternion fromAxisAndAngle(const GLMVector3D &axis, float angle_deg);
        static GLMQuaternion fromAxisAndAngle(float x, float y, float z, float angle_deg);
        static GLMQuaternion slerp(const GLMQuaternion &lhs, const GLMQuaternion &rhs, float t);
        GLMQuaternion() = default;
        GLMQuaternion(float scalar, float x, float y, float z);
        GLMQuaternion(const GLMQuaternion &quaternion);
        GLMQuaternion &operator=(const glm::quat &quaternion);
        GLMQuaternion(const glm::quat& quaternion);
        bool isIdentity() const;
        float length() const;
        float lengthSquared() const;
        GLMQuaternion conjugated() const;
        void normalize();
        GLMQuaternion normalized() const;
        GLMQuaternion inverted() const;
        GLMVector3D rotatedVector(const GLMVector3D &vector) const;
        bool isNormalized() const;
        bool isNull() const;
        void setX(float x);
        float getX() const;
        void setY(float y);
        float getY() const;
        void setZ(float z);
        float getZ() const;
        void setScalar(float w);
        float getScalar() const;
        GLMVector3D toVector3D() const;
        GLMVector4D toVector4D() const;
        GLMMatrix3x3 toMatrix3x3() const;
        GLMMatrix4x4 toMatrix4x4() const;
        void getAxisAndAngle(float *x, float *y, float *z, float *angle_deg);
    };
}