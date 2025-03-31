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
}

void lcf::ThirdPersonController::processInput()
{

}

void lcf::ThirdPersonController::updateCameraPitch(Camera *camera, float angle_deg)
{

}

void lcf::ThirdPersonController::updateCameraYaw(Camera *camera, float angle_deg)
{

}