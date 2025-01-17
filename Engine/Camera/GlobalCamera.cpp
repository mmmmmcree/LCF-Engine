#include "GlobalCamera.h"
#include "CameraController.h"

lcf::GlobalCamera *lcf::GlobalCamera::instance()
{
    static GlobalCamera s_instance;
    return &s_instance;
}

void lcf::GlobalCamera::bind()
{
    m_controller->update(this);
    Camera::bind();
}

void lcf::GlobalCamera::setControllerType(ControllerType type)
{
    static GameController s_game_controller;
    static TrackballController s_trackball_controller;
    switch (type) {
        case ControllerType::Game : { m_controller = &s_game_controller; } break;
        case ControllerType::Trackball : { m_controller = &s_trackball_controller; } break;
    }
}

void lcf::GlobalCamera::setCameraType(Type type)
{
    switch (type) {
        case TrackballPerspective : {
            this->setProjectionType(ProjectionType::Perspective);
            this->setControllerType(ControllerType::Trackball);
        } break;
        case FirstPersonPerspective : {
            this->setProjectionType(ProjectionType::Perspective);
            this->setControllerType(ControllerType::Game);
        } break;
    }
}

void lcf::GlobalCamera::reset()
{
    this->setTranslation({0.0f, 0.0f, 5.0f});
    this->right() = {1.0f, 0.0f, 0.0f};
    this->up() = {0.0f, 1.0f, 0.0f};
}

void lcf::GlobalCamera::processMouseMoveEvent(QMouseEvent *event)
{
    m_controller->processMouseMoveEvent(event);
}

void lcf::GlobalCamera::processKeyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_R : { this->reset(); } break;
    }
    m_controller->processKeyPressEvent(event);
}

void lcf::GlobalCamera::processKeyReleaseEvent(QKeyEvent *event)
{
    m_controller->processKeyReleaseEvent(event);
}

void lcf::GlobalCamera::processWheelEvent(QWheelEvent *event)
{
    m_controller->processWheelEvent(event);
}

lcf::GlobalCamera::GlobalCamera()
{
    this->setControllerType(ControllerType::Trackball);
    this->reset();
}
