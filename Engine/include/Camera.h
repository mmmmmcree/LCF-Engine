#pragma once

#include "Object3D.h"
#include "ProjectionProvider.h"
#include "UniformBufferObject.h"

namespace lcf {
    class Camera : public Object3D
    {
    public:
        using ProjectionType = ProjectionProvider::Type;
        Camera() = default;
        ~Camera() = default;
        void bind();
        Vector3D front();
        Vector3D up() const;
        Vector3D right() const;
        void setProjectionType(ProjectionType type);
        void setViewport(int width, int height);
        const Matrix4x4 &getViewMatrix() const;
        const Matrix4x4 &getProjectionMatrix() const;
        const Matrix4x4 &getProjectionViewMatrix() const;
    protected:
        UniformBufferObject m_ubo;
        Vector3D m_up = {0, 1, 0};
        ProjectionProvider m_projection_provider;
        Matrix4x4 m_projection_view;
    };
}