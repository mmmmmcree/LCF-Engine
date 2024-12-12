#pragma once

#include "CameraController.h"

namespace lcf {
    class GameController : public CameraController
    {
        enum MoveDirection {
            None = 0,
            Forward = 1,
            Backward = 1 << 1,
            Left = 1 << 2,
            Right = 1 << 3,
            Up = 1 << 4,
            Down = 1 << 5
        };
    public:
        GameController();
        void update(Camera *camera) override;
        void processMouseMoveEvent(QMouseEvent *event) override;
        void processKeyPressEvent(QKeyEvent *event) override;
        void processKeyReleaseEvent(QKeyEvent *event) override;
    private:
        void updateCameraPitch(Camera *camera, double angle_deg);
        void updateCameraYaw(Camera *camera, double angle_deg);
    private:
        float m_pitch = 0.0f;
        float m_delta_pitch = 0.0f;
        float m_delta_yaw = 0.0f;
        int m_direction = None;
    };
}