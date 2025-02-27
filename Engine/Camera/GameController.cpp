#include "GameController.h"
#include "InputProcessor.h"

lcf::GameController::GameController()
{
    this->setSensitivity(0.15f);
    this->setMoveSpeed(0.1f);
}

void lcf::GameController::update(Camera *camera)
{
    this->processInput(camera);
    this->updateCameraPitch(camera, m_delta_pitch);
    this->updateCameraYaw(camera, m_delta_yaw);
    camera->translate(m_direction * m_move_speed);
    m_delta_pitch = m_delta_yaw = 0.0f;
}

void lcf::GameController::processInput(Camera * camera)
{
    auto input_processor = InputProcessor::instance();
    auto [dx, dy] = input_processor->getMouseOffset();
    if (input_processor->isRightMouseButtonPressed()) {
        m_delta_yaw -= dx * m_sensitivity;
        m_delta_pitch -= dy * m_sensitivity;   
    }
    m_direction = input_processor->getMovingDirection(camera->up(), camera->right());
}

void lcf::GameController::updateCameraPitch(Camera *camera, float angle_deg)
{
    float temp_pitch = m_pitch + angle_deg;
    if (temp_pitch > 89.0f or temp_pitch < -89.0f) { return; }
    m_pitch = temp_pitch;
    Quaternion pitch = Quaternion::fromAxisAndAngle(camera->right(), angle_deg);
    camera->setUp(pitch * camera->up());
}

void lcf::GameController::updateCameraYaw(Camera *camera, float angle_deg)
{
    Quaternion yaw = Quaternion::fromAxisAndAngle(Vector3D(0.0f, 1.0f, 0.0f), angle_deg);
    camera->setRight(yaw * camera->right());
    camera->setUp(yaw * camera->up());
}
