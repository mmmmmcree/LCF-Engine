#pragma once

#include "CameraController.h"

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
    void update() override;
    Type type() const override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processKeyPressEvent(QKeyEvent *event) override;
    void processKeyReleaseEvent(QKeyEvent *event) override;
private:
    void updateCameraPitch(double angle_deg);
    void updateCameraYaw(double angle_deg);
private:
    float m_pitch = 0.0f;
    int m_direction = None;
};