#include "ProjectionProvider.h"

ProjectionProvider::ProjectionProvider() :
    m_fov(45.0f), m_aspect(1.0f), m_near(0.1f), m_far(1000.0f),
    m_ortho_size(7.0f), 
    m_scale(0.0f), m_scale_speed(0.0005f), 
    m_type(Perspective)
{
}

const QMatrix4x4 &ProjectionProvider::projectionMatrix() const
{
    return m_projection;
}

void ProjectionProvider::setType(Type type)
{
    m_type = type;
    this->update();
}

void ProjectionProvider::setOrthoSize(float size)
{
    m_ortho_size = size;
    this->update();
}

void ProjectionProvider::setNearPlane(float near_plane)
{
    m_near = near_plane;
    this->update();
}

void ProjectionProvider::setFarPlane(float far_plane)
{
    m_far = far_plane;
}

void ProjectionProvider::setFov(float fov_deg)
{
    m_fov = fov_deg;
    if (m_type == Perspective) {
        this->update();
    }
}

ProjectionProvider::Type ProjectionProvider::type() const
{
    return m_type;
}

void ProjectionProvider::update()
{
    m_projection.setToIdentity();
    if (m_type == Orthographic) {
        float scale = qPow(2.0f, m_scale);
        m_projection.ortho(-m_ortho_size * scale, m_ortho_size * scale, -m_ortho_size * scale, m_ortho_size * scale, m_near, m_far);
    } else {
        m_projection.perspective(m_fov, m_aspect, m_near, m_far);
    }
}

void ProjectionProvider::setAspect(int width, int height)
{
    m_aspect = static_cast<float>(width) / static_cast<float>(height);
    if (m_type == Perspective) {
        this->update();
    }
}

void ProjectionProvider::increaseScale(float delta)
{
    m_scale += delta * m_scale_speed;
    this->update();
}

const float & ProjectionProvider::farPlane() const
{
    return m_far;
}
