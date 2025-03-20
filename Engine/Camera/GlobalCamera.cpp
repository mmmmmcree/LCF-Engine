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
    static ThirdPersonController s_third_person_controller;
    switch (type) {
        case ControllerType::Game : { m_controller = &s_game_controller; } break;
        case ControllerType::Trackball : { m_controller = &s_trackball_controller; } break;
        case ControllerType::ThirdPerson : { m_controller = &s_third_person_controller; } break;
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
    m_transformer.setPosition({0.0f, 0.0f, 10.0f});
    this->setRight({1.0f, 0.0f, 0.0f});
    this->setUp({0.0f, 1.0f, 0.0f});
}

lcf::GlobalCamera::GlobalCamera()
{
    this->setControllerType(ControllerType::Trackball);
    this->reset();
}
