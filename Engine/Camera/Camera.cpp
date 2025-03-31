#pragma once

#include "Camera.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>

void lcf::Camera::bind()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    if (not m_ubo) {
        m_ubo.setBindingPoint(0);
        m_ubo.setDataSizes({64, 64, 16, 64});
        m_ubo.create();
    }
    if (m_transformer.isUpdated()) { return; }
    Vector3D position = this->getWorldPosition();
    m_projection_view = m_projection_provider.projectionMatrix() * this->getViewMatrix();
    m_ubo.bind();
    m_ubo.updateData(0, this->getViewMatrix().constData());
    m_ubo.updateData(1, m_projection_provider.projectionMatrix().constData());
    m_ubo.updateData(2, &position);
    m_ubo.updateData(3, m_projection_view.constData());
    m_ubo.release();
}

lcf::Vector3D lcf::Camera::front()
{
    return -m_transformer.getZAxis();
}

lcf::Vector3D lcf::Camera::up() const
{
    return m_transformer.getYAxis();
}

lcf::Vector3D lcf::Camera::right() const
{
    return m_transformer.getXAxis();
}

void lcf::Camera::setProjectionType(ProjectionType type)
{
    m_projection_provider.setType(type);
}

void lcf::Camera::setViewport(int width, int height)
{
    m_projection_provider.setAspect(width, height);
}

const lcf::Matrix4x4 &lcf::Camera::getViewMatrix() const
{
    return this->getInversedWorldMatrix();
}

const lcf::Matrix4x4 &lcf::Camera::getProjectionMatrix() const
{
    return m_projection_provider.projectionMatrix();
}

const lcf::Matrix4x4 &lcf::Camera::getProjectionViewMatrix() const
{
    return m_projection_view;
}