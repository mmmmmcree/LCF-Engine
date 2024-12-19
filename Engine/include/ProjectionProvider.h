#pragma once

#include <QMatrix4x4>

class ProjectionProvider
{
public:
    enum Type {
        Perspective,
        Orthographic
    };
    ProjectionProvider();
    const QMatrix4x4 &projectionMatrix() const;
    void setType(Type type);
    void setOrthoSize(float size);
    void setNearPlane(float near_plane);
    void setFarPlane(float far_plane);
    Type type() const;
    void update();
    void setAspect(int width, int height);
    void increaseScale(float delta);
private:
    Type m_type;
    float m_fov, m_aspect, m_near, m_far;
    // float m_left, m_right, m_bottom, m_top;
    float m_ortho_size;
    float m_scale, m_scale_speed;
    QMatrix4x4 m_projection;
};