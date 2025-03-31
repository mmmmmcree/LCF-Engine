#pragma once

#include "Matrix.h"

class ProjectionProvider
{
public:
    enum Type {
        Perspective,
        Orthographic
    };
    ProjectionProvider();
    ProjectionProvider(const ProjectionProvider &other) = default;
    const lcf::Matrix4x4 &projectionMatrix() const;
    void setType(Type type);
    void setOrthoSize(float size);
    void setNearPlane(float near_plane);
    void setFarPlane(float far_plane);
    void setFov(float fov_deg);
    Type type() const;
    void update();
    void setAspect(int width, int height);
    void increaseScale(float delta);
    const float &farPlane() const;
    const float &nearPlane() const;
    float nearPlaneWidth() const;
private:
    Type m_type;
    float m_fov, m_aspect, m_near, m_far;
    float m_ortho_size;
    float m_scale, m_scale_speed;
    lcf::Matrix4x4 m_projection;
};