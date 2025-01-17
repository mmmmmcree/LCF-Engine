#pragma once

#include "Matrix.h"
#include "GLShaderProgram.h"
#include "Define.h"
#include "SignalSender.h"

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
        Object3D *root() const;
        Object3D *parent() const;
        void setParent(Object3D* parent);
        void addChild(Object3D* child);
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
        const std::vector<Object3D*> &children() const;
        const Matrix4x4 &worldMatrix();
        const Matrix4x4  &inversedWorldMatrix();
        Matrix3x3 normalMatrix();
        void setName(std::string_view name);
        const std::string &name() const;
        void setCastShadow(bool cast_shadow);
        bool castShadow() const;
        void setSignalSender(SignalSender *signal_sender);
        SignalSender *signalSender() const;
    protected:
        void updateWorldMatrix();
        void addChildToChildren(Object3D* child);
        void removeChild(Object3D* child);
    protected:
        //! 初始化默认需要更新，否则在没有设置父节点的情况下，即将成为父节点的节点的更新需求不会传递到该节点
        Matrix4x4 m_local;
        Matrix4x4 m_world;
        Matrix4x4 m_inversed_world;
        DecomposedTransform m_local_decomposed;
        bool m_world_need_update = true;
        bool m_normal_matrix_need_update = true;
        Object3D *m_parent = nullptr;
        std::string m_name;
        bool m_cast_shadow = false;
        SignalSender *m_signal_sender = nullptr;
    private:
        std::vector<Object3D*> m_children;
    };

    using Bone = Object3D;

    using Group = Object3D;

}
