#pragma once

#include "Camera.h"
#include <memory>
#include "TrackballController.h"
#include "GameController.h"

Camera *Camera::get()
{
    static std::unique_ptr<Camera> s_instance(nullptr);
    if (not s_instance) {
        s_instance.reset(new Camera);
    }
    return s_instance.get();
}

void Camera::initialize()
{
    QOpenGLContext::currentContext()->extraFunctions()->glGenBuffers(1, &m_ubo);
}

const QVector3D &Camera::position() const
{
    return m_controller->position();
}

void Camera::update()
{
    m_controller->update();
    this->setUBO();
}

void Camera::setViewPort(int width, int height)
{
    m_projection_provider.setAspect(width, height);
}

void Camera::setCameraType(Type type)
{
    switch (type) {
        case TrackballPerspective : {
            this->setProjectionType(ProjectionType::Perspective);
            this->setCameraController(ControllerType::Trackball);
        } break;
        case FirstPersonPerspective : {
            this->setProjectionType(ProjectionType::Perspective);
            this->setCameraController(ControllerType::Game);
        } break;
    }
}

void Camera::setCameraController(ControllerType type)
{
    static std::unique_ptr<TrackballController> track_ball_controller(new TrackballController);
    static std::unique_ptr<GameController> game_controller(new GameController);
    switch (type) {
        case ControllerType::Trackball : { m_controller = track_ball_controller.get(); break; }
        case ControllerType::Game : { m_controller = game_controller.get(); break; }
    }
}

void Camera::setProjectionType(ProjectionType type)
{
    m_projection_provider.setType(type);
}

void Camera::processMouseMoveEvent(QMouseEvent *event)
{
    m_controller->processMouseMoveEvent(event);
}

void Camera::processKeyPressEvent(QKeyEvent *event)
{
    m_controller->processKeyPressEvent(event);
}

void Camera::processKeyReleaseEvent(QKeyEvent *event)
{
    m_controller->processKeyReleaseEvent(event);
}

void Camera::processWheelEvent(QWheelEvent *event)
{
    if (m_projection_provider.type() == ProjectionType::Orthographic) {
        m_projection_provider.increaseScale(-event->angleDelta().y());
    } else {
        m_controller->processWheelEvent(event);
    }
}

Camera::Camera()
{
    this->setCameraController(ControllerType::Trackball);
}

void Camera::setUBO()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    gl->glBufferData(GL_UNIFORM_BUFFER, 144, nullptr, GL_DYNAMIC_DRAW);
    gl->glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo, 0, 144);
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 0, 64, m_controller->viewMatrix().constData());
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 64, 64, m_projection_provider.projectionMatrix().constData());
    gl->glBufferSubData(GL_UNIFORM_BUFFER, 128, 16, &m_controller->position());
    gl->glBindBuffer(GL_UNIFORM_BUFFER, 0);
}