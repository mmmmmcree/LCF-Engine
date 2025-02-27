#include "CameraController.h"

void lcf::CameraController::setSensitivity(float sensitivity)
{
    m_sensitivity = sensitivity;
}

void lcf::CameraController::setMoveSpeed(float speed)
{
    m_move_speed = speed;
}

void lcf::CameraController::setScaleSpeed(float speed)
{
    m_scale_speed = speed;
}
