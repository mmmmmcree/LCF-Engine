#pragma once

#include "CameraController.h"

class TrackballController : public CameraController
{
public:
    TrackballController();
    Type type() const override;
    void processMouseMoveEvent(QMouseEvent *event) override;
    void processWheelEvent(QWheelEvent *event) override;
private:
    void updateCameraPitch(double angle_deg);
    void updateCameraYaw(double angle_deg);
};