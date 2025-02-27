#include "ThirdPersonController.h"
#include "InputProcessor.h"
#include "ControlManager.h"

lcf::ThirdPersonController::ThirdPersonController() : CameraController()
{
    this->setSensitivity(0.2f);
    this->setMoveSpeed(0.005f);
    this->setScaleSpeed(0.005f);
}

void lcf::ThirdPersonController::update(Camera *camera)
{
    auto model = ControlManager::instance()->currentModel();
    if (model != camera->parent()) {
        camera->setTranslation(0.0f, 0.0f, 0.0f);
        camera->setParent(model);
    }
    this->processInput();
    this->updateCameraYaw(camera, m_delta_yaw);
    this->updateCameraPitch(camera, m_delta_pitch);
    // camera->translate(camera->right() * m_delta_right + camera->up() * m_delta_up + camera->front() * m_delta_front);
    camera->translate(camera->front() * m_delta_front);
    // if (camera->parent()){
    //     Vector3D front = -model->worldMatrix().column(2).toVector3D().normalized();
    //     camera->setRight(Vector3D::crossProduct(front, camera->up()).normalized());
    // }
    m_delta_yaw = m_delta_pitch = m_delta_up = m_delta_right = m_delta_front = 0.0f;
}

void lcf::ThirdPersonController::processInput()
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

void lcf::ThirdPersonController::updateCameraPitch(Camera *camera, float angle_deg)
{
    Quaternion pitch = Quaternion::fromAxisAndAngle(camera->right(), angle_deg);
    camera->setUp(pitch * camera->up());
    camera->translate(pitch * camera->localPosition() - camera->localPosition());
}

void lcf::ThirdPersonController::updateCameraYaw(Camera *camera, float angle_deg)
{
    Quaternion yaw = Quaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, angle_deg);
    camera->setUp(yaw * camera->up());
    camera->setRight(yaw * camera->right());
    camera->translate(yaw * camera->localPosition() - camera->localPosition());
}