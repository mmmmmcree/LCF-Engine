#pragma once

#include "CameraController.h"

namespace lcf {
    class TrackballController : public CameraController
    {
    public:
        TrackballController();
        void update(Camera *camera) override;
    private:
        void processInput();
        void updateCameraPitch(Camera *camera, float angle_deg);
        void updateCameraYaw(Camera *camera, float angle_deg);
    private:
        float m_delta_right = 0.0f;
        float m_delta_up = 0.0f;
        float m_delta_front = 0.0f;
        float m_delta_pitch = 0.0f;
        float m_delta_yaw = 0.0f;
    };
}