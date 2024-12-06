#pragma once

#include "Matrix.h"
#include "Material.h"

namespace lcf {
    class Object3D
    {
    public:
        using SharedPtr = std::shared_ptr<Object3D>;
        enum class Type
        {
            Group,
            Mesh,
            Model,
        };
        Object3D() = default;
        Object3D(const Object3D &other);
        virtual ~Object3D() = default;
        virtual void draw();
        virtual Type type() const;
        void debugParent() const;
        Object3D *root() const;
        void setParent(Object3D* parent);
        void addChild(Object3D* child);
        void setLocalMatrix(const Matrix4x4 &matrix);
        void translate(float x, float y, float z);
        void translateX(float x);
        void translateY(float y);
        void translateZ(float z);
        void translate(const Vector3D &translation);
        void setTranslation(float x, float y, float z);
        void setTranslation(const Vector3D &translation);
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
        const std::vector<Object3D*> &children() const;
        virtual const Matrix4x4 &worldMatrix();
        void setName(std::string_view name);
        const std::string &name() const;
    protected:
        void updateWorldMatrix();
        void notifyWorldMatrixUpdatedToChildren();
        void addChildToChildren(Object3D* child);
        void removeChild(Object3D* child);
    protected:
        //! 初始化默认需要更新，否则在没有设置父节点的情况下，即将成为父节点的节点的更新需求不会传递到该节点
        bool m_world_need_update = true;
        Matrix4x4 m_local;
        DecomposedTransform m_local_decomposed;
        Matrix4x4 m_world;
        Object3D *m_parent = nullptr;
        std::string m_name;
    private:
        std::vector<Object3D*> m_children;
    };

    using Bone = Object3D;

    using Group = Object3D;

}
