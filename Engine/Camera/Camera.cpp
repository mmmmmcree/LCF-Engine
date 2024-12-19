#pragma once

#include "Camera.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

void lcf::Camera::bind()
{
    m_view.setToIdentity();
    m_view.lookAt(this->position(), this->position() + this->front(), m_up);
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    if (not m_ubo) {
        gl->glGenBuffers(1, &m_ubo);
        gl->glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
        gl->glBufferData(GL_UNIFORM_BUFFER, 144, nullptr, GL_DYNAMIC_DRAW);
    }
    gl->glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    gl->glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo, 0, 144);
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, m_view.constData());
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, m_projection_provider.projectionMatrix().constData());
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 128, 16, &this->position());
    gl->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

lcf::Vector3D lcf::Camera::front()
{
    return Vector3D::crossProduct(m_up, m_right);
}

const lcf::Vector3D &lcf::Camera::up() const
{
    return m_up;
}

lcf::Vector3D &lcf::Camera::up()
{
    return m_up;
}

const lcf::Vector3D &lcf::Camera::right() const
{
    return m_right;
}

lcf::Vector3D &lcf::Camera::right()
{
    return m_right;
}

void lcf::Camera::setProjectionType(ProjectionType type)
{
    m_projection_provider.setType(type);
}

void lcf::Camera::setViewPort(int width, int height)
{
    m_projection_provider.setAspect(width, height);
}
