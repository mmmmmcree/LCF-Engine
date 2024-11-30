#pragma once

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMatrix4x4>

class CameraController
{
public:
    enum Type {
        Trackball,
        Game
    };
    CameraController() = default;
    virtual ~CameraController() = default;
    const QMatrix4x4 &viewMatrix();
    const QVector3D &position() const;
    virtual void update() { }
    virtual Type type() const = 0;
    virtual void reset();
    virtual void processMouseMoveEvent(QMouseEvent* event) = 0;
    virtual void processKeyPressEvent(QKeyEvent* event);
    virtual void processKeyReleaseEvent(QKeyEvent* event);
    virtual void processWheelEvent(QWheelEvent* event) { }
    void setSensitivity(float sensitivity);
    void setMoveSpeed(float speed);
    void setScaleSpeed(float speed);
protected:
    QVector3D front() const;
protected:
    QVector3D m_position, m_right, m_up;
    QMatrix4x4 m_view;
    float m_sensitivity = 0.2f, m_move_speed = 0.005f, m_scale_speed = 0.01f;
    QPointF m_last_mouse_pos;
};