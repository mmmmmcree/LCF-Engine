#pragma once

#include "CameraController.h"

namespace lcf {
    class GameController : public CameraController
    {
    public:
        GameController();
        void update(Camera *camera) override;
    private:
        void processInput(Camera *camera);
        void updateCameraPitch(Camera *camera, float angle_deg);
        void updateCameraYaw(Camera *camera, float angle_deg);
    private:
        float m_pitch = 0.0f;
        float m_yaw = 0.0f;
        float m_delta_pitch = 0.0f;
        float m_delta_yaw = 0.0f;
        Vector3D m_direction;
    };
}