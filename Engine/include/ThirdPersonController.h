#pragma once

#include "CameraController.h"

namespace lcf {
    class ThirdPersonController : public CameraController
    {
    public:
        ThirdPersonController();
        void update(Camera *camera) override;
    private:
        void processInput();
        void updateCameraPitch(Camera *camera, float angle_deg);
        void updateCameraYaw(Camera *camera, float angle_deg);
    private:
        float m_front = 0.0f;
        float m_delta_front = 0.0f;
        float m_pitch = 0.0f;
        float m_delta_pitch = 0.0f;
        float m_yaw = 0.0f;
        float m_delta_yaw = 0.0f;
        float m_right = 0.0f;
        float m_delta_right = 0.0f;
        float m_up = 0.0f;
        float m_delta_up = 0.0f;
    };
}