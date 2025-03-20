#pragma once

#include "Matrix.h"
#include "GLShaderProgram.h"
#include "Define.h"
#include "SingleUniform.h"
#include "Hierarchical.h"
#include "HierarchicalTransformer.h"
#include <QObject>
#include "Bone.h"

namespace lcf {
    class Object3D
    {
    public:
        using SharedPtr = std::shared_ptr<Object3D>;
        Object3D() = default;
        Object3D(const Object3D &other);
        virtual ~Object3D() = default;
        virtual void draw();
        virtual void drawShadow(LightType light_type);
        virtual Object3DType type() const;
        void attachTo(Object3D* parent);
        void setLocalMatrix(const Matrix4x4 &matrix);
        void translate(float x, float y, float z);
        void translateX(float x);
        void translateY(float y);
        void translateZ(float z);
        void translate(const Vector3D &translation);
        void setTranslation(float x, float y, float z);
        void setTranslation(const Vector3D &position);
        void rotate(const Quaternion &rotation);
        void rotate(float angle_deg, float x, float y, float z);
        void setRotation(float angle_deg, float x, float y, float z);
        void setRotation(const Quaternion &rotation);
        void rotateX(float angle_deg);
        void setRotationX(float angle_deg);
        void rotateY(float angle_deg);
        void setRotationY(float angle_deg);
        void rotateZ(float angle_deg);
        void setRotationZ(float angle_deg);
        void scale(float x, float y, float z);
        void scale(float factor);
        void scale(const Vector3D &scale);
        void setScale(float x, float y, float z);
        void setScale(const Vector3D &scale);
        void setScale(float factor);
        const Vector3D &localPosition() const;
        Vector3D worldPosition();
        const Vector3D &translation() const;
        const Quaternion &rotation() const;
        const Vector3D &scale() const;
        const Matrix4x4 &worldMatrix();
        const Matrix4x4  &inversedWorldMatrix();
        Matrix3x3 normalMatrix();
        virtual void setName(std::string_view name);
        const std::string &name() const;
    protected:
        std::string m_name;
        HierarchicalTransformer m_transformer;
    };
}
