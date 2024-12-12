#pragma once

#include "Obejct3D.h"
#include "ProjectionProvider.h"

namespace lcf {
    class Camera : public Object3D
    {
    public:
        using ProjectionType = ProjectionProvider::Type;
        Camera() = default;
        void bind();
        Vector3D front();
        const Vector3D &up() const;
        Vector3D &up();
        const Vector3D &right() const;
        Vector3D &right();
        void setProjectionType(ProjectionType type);
        void setViewPort(int width, int height);
    private:
        GLuint m_ubo = 0;
        Vector3D m_up = {0, 1, 0};
        Vector3D m_right = {1, 0, 0};
        Matrix4x4 m_view;
        ProjectionProvider m_projection_provider;
    };
}