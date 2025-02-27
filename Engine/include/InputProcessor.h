#pragma once

#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include "Matrix.h"

namespace lcf {
    
    class InputProcessor : public QObject
    {
        Q_OBJECT
        enum MovingDirection
        {
            None = 0,
            Forward = 1,
            Backward = 1 << 1,
            Left = 1 << 2,
            Right = 1 << 3,
            Up = 1 << 4,
            Down = 1 << 5
        };
    public:
        static InputProcessor *instance();
        void processMouseMoveEvent(QMouseEvent* event);
        void processKeyEvent(QKeyEvent* event);
        void processWheelEvent(QWheelEvent* event);
        Vector2D getMouseOffset();
        Vector2D getWheelOffset();
        Vector3D getMovingDirection(const Vector3D &up, const Vector3D &right) const;
        bool isMovingForward() const;
        bool isMovingBackward() const;
        bool isMovingLeft() const;
        bool isMovingRight() const;
        bool isMovingUp() const;
        bool isMovingDown() const;
        bool isLeftMouseButtonPressed() const;
        bool isRightMouseButtonPressed() const;
    private:
        void processKeyPressedEvent(QKeyEvent* event);
        void processKeyReleasedEvent(QKeyEvent* event);
    private:
        InputProcessor() = default;
    private:
        Vector2D m_previous_mouse_pos;
        Vector2D m_accumulated_mouse_offset;
        Vector2D m_accumulated_wheel_offset;
        int m_direction = None;
        Qt::MouseButtons m_buttons;
    };
}