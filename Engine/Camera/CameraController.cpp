#include "CameraController.h"

const QMatrix4x4 &CameraController::viewMatrix()
{
    m_view.setToIdentity();
    m_view.lookAt(m_position, m_position + this->front(), m_up);
    return m_view;
}

const QVector3D &CameraController::position() const
{
    return m_position;
}

void CameraController::reset()
{
    m_position = {0.0f, 0.0f, 5.0f};
    m_right = {1.0f, 0.0f, 0.0f};
    m_up = {0.0f, 1.0f, 0.0f};
}

void CameraController::processKeyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) { return; }
    switch (event->key()) {
        case Qt::Key_R : { this->reset(); } break;
    }
}

void CameraController::processKeyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) { return; }
}

void CameraController::setSensitivity(float sensitivity)
{
    m_sensitivity = sensitivity;
}

void CameraController::setMoveSpeed(float speed)
{
    m_move_speed = speed;
}

void CameraController::setScaleSpeed(float speed)
{
    m_scale_speed = speed;
}

QVector3D CameraController::front() const
{
    return QVector3D::crossProduct(m_up, m_right);
}
