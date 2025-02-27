#pragma once

#include "Camera.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>

void lcf::Camera::bind()
{
    m_view.setToIdentity();
    m_view.lookAt(this->worldPosition(), this->worldPosition() + this->front(), m_up);
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    if (not m_ubo) {
        m_ubo.setBindingPoint(0);
        m_ubo.setDataSizes({64, 64, 16});
        m_ubo.create();
    }
    m_ubo.bind();
    m_ubo.updateData(0, m_view.constData());
    m_ubo.updateData(1, m_projection_provider.projectionMatrix().constData());
    Vector3D position = this->worldPosition();
    m_ubo.updateData(2, &position);
    m_ubo.release();
}

lcf::Vector3D lcf::Camera::front()
{
    return Vector3D::crossProduct(m_up, m_right);
}

void lcf::Camera::setUp(const Vector3D &up)
{
    m_up = up;
}

const lcf::Vector3D &lcf::Camera::up() const
{
    return m_up;
}

void lcf::Camera::setRight(const Vector3D &right)
{
    m_right = right;
}

const lcf::Vector3D &lcf::Camera::right() const
{
    return m_right;
}

void lcf::Camera::setProjectionType(ProjectionType type)
{
    m_projection_provider.setType(type);
}

void lcf::Camera::setViewport(int width, int height)
{
    m_projection_provider.setAspect(width, height);
}
