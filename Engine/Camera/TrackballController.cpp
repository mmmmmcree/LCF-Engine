#include "TrackBallController.h"
#include "InputProcessor.h"

lcf::TrackballController::TrackballController() : CameraController()
{
    this->setSensitivity(0.2f);
    this->setMoveSpeed(0.005f);
    this->setScaleSpeed(0.005f);
}

void lcf::TrackballController::update(Camera *camera)
{
    this->processInput();
    this->updateCameraYaw(camera, m_delta_yaw);
    this->updateCameraPitch(camera, m_delta_pitch);
    camera->translate(camera->right() * m_delta_right + camera->up() * m_delta_up + camera->front() * m_delta_front);
    m_delta_yaw = m_delta_pitch = m_delta_up = m_delta_right = m_delta_front = 0.0f;
}

void lcf::TrackballController::processInput()
{
    auto input_processor = InputProcessor::instance();
    auto [dx, dy] = input_processor->getMouseOffset();
    if (input_processor->isLeftMouseButtonPressed()) {
        m_delta_yaw -= dx * m_sensitivity;
        m_delta_pitch -= dy * m_sensitivity;
    }
    if (input_processor->isRightMouseButtonPressed()) {
        m_delta_right -= dx * m_move_speed;
        m_delta_up += dy * m_move_speed;
    }
    m_delta_front += input_processor->getWheelOffset().y() * m_scale_speed;
}

void lcf::TrackballController::updateCameraPitch(Camera *camera, float angle_deg)
{
    Quaternion pitch = Quaternion::fromAxisAndAngle(camera->right(), angle_deg);
    camera->setUp(pitch * camera->up());
    camera->translate(pitch * camera->localPosition() - camera->localPosition());
}

void lcf::TrackballController::updateCameraYaw(Camera *camera, float angle_deg)
{
    Quaternion yaw = Quaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, angle_deg);
    camera->setUp(yaw * camera->up());
    camera->setRight(yaw * camera->right());
    camera->translate(yaw * camera->localPosition() - camera->localPosition());
}
