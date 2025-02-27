#pragma once

#include "Matrix.h"
#include "Camera.h"

namespace lcf {
    class CameraController
    {
    public:
        enum Type
        {
            Trackball,
            Game,
            ThirdPerson,
        };
        CameraController() = default;
        virtual ~CameraController() = default;
        virtual void update(Camera *camera) = 0;
        void setSensitivity(float sensitivity);
        void setMoveSpeed(float speed);
        void setScaleSpeed(float speed);
    protected:
        float m_sensitivity = 0.2f, m_move_speed = 0.005f, m_scale_speed = 0.01f;
        QPointF m_last_mouse_pos;
    };
}