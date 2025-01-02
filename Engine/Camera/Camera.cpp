#pragma once

#include "Camera.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>

void lcf::Camera::bind()
{
    m_view.setToIdentity();
    m_view.lookAt(this->localPosition(), this->localPosition() + this->front(), m_up);
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    if (not m_ubo) {
        m_ubo.setBindingPoint(0);
        m_ubo.setDataSizes({64, 64, 16});
        m_ubo.create();
    }
    m_ubo.bind();
    m_ubo.updateData(0, m_view.constData());
    m_ubo.updateData(1, m_projection_provider.projectionMatrix().constData());
    m_ubo.updateData(2, &this->localPosition());
    m_ubo.release();
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

void lcf::Camera::setViewport(int width, int height)
{
    m_projection_provider.setAspect(width, height);
}
