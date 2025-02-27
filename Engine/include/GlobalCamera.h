#pragma once

#include "Camera.h"
#include "GameController.h"
#include "TrackballController.h"
#include "ThirdPersonController.h"

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
    private:
        GlobalCamera();
    private:
        CameraController *m_controller = nullptr;
    };
}