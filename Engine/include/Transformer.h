#pragma once

#include "Matrix.h"

namespace lcf {
    class Transformer
    {
    public:
        Transformer() = default;
        Transformer(const Transformer &other);
        Transformer &operator=(const Transformer &other);
        void setMatrix(const Matrix4x4 &matrix);
        const Matrix4x4 &getMatrix();
        void translate(float x, float y, float z);
        void translate(const Vector3D &translation);
        void rotate(float angle_degrees, float x, float y, float z);
        void rotate(float angle_degrees, const Vector3D &axis);
        void rotate(const Quaternion &rotation);
        void rotateXAxis(float angle_degrees);
        void rotateYAxis(float angle_degrees);
        void rotateZAxis(float angle_degrees);
        void scale(float factor);
        void scale(float x, float y, float z);
        void scale(const Vector3D &scale);
        void setPosition(float x, float y, float z);
        void setPosition(const Vector3D &position);
        void setRotation(float angle_degrees, float x, float y, float z);
        void setRotation(float angle_degrees, const Vector3D &axis);
        void setRotation(const Quaternion &rotation);
        void setRotationXAxis(float angle_degrees);
        void setRotationYAxis(float angle_degrees);
        void setRotationZAxis(float angle_degrees);
        void setScale(float x, float y, float z);
        void setScale(const Vector3D &scale);
        void setScale(float factor);
        Vector3D getXAxis();
        Vector3D getYAxis();
        Vector3D getZAxis();
        const Vector3D &getPosition() const;
        const Quaternion &getRotation() const;
        const Vector3D &getScale() const;
        bool isUpdated() const;
    protected:
        virtual void requireUpdate();
    protected:
        bool m_need_update = true;
        Vector3D m_position;
        Quaternion m_rotation;
        Vector3D m_scale = Vector3D(1.0f, 1.0f, 1.0f);
        Matrix4x4 m_matrix;
    };
}