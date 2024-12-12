#pragma once

#include "Camera.h"
#include "GameController.h"
#include "TrackballController.h"

namespace lcf {
    class GlobalCamera : public Camera
    {
    public:
        using ControllerType = CameraController::Type;
        enum Type
        {
            TrackballPerspective, 
            FirstPersonPerspective, 
        };
        static GlobalCamera *instance();
        void bind();
        void setControllerType(ControllerType type);
        void setCameraType(Type type);
        void reset();
        void processMouseMoveEvent(QMouseEvent *event);
        void processKeyPressEvent(QKeyEvent *event);
        void processKeyReleaseEvent(QKeyEvent *event);
        void processWheelEvent(QWheelEvent *event);
    private:
        GlobalCamera();
    private:
        CameraController *m_controller = nullptr;
    };
}