#pragma once

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include "Matrix.h"
#include "Camera.h"

namespace lcf {
    class CameraController
    {
    public:
        enum Type
        {
            Trackball,
            Game
        };
        CameraController() = default;
        virtual ~CameraController() = default;
        virtual void update(Camera *camera) = 0;
        virtual void processMouseMoveEvent(QMouseEvent* event) = 0;
        virtual void processKeyPressEvent(QKeyEvent* event);
        virtual void processKeyReleaseEvent(QKeyEvent* event);
        virtual void processWheelEvent(QWheelEvent* event) { }
        void setSensitivity(float sensitivity);
        void setMoveSpeed(float speed);
        void setScaleSpeed(float speed);
    protected:
        float m_sensitivity = 0.2f, m_move_speed = 0.005f, m_scale_speed = 0.01f;
        QPointF m_last_mouse_pos;
    };
}