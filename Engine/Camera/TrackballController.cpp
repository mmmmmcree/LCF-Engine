#include "TrackBallController.h"

TrackballController::TrackballController() : CameraController()
{
    this->reset();
    this->setSensitivity(0.2f);
    this->setMoveSpeed(0.005f);
    this->setScaleSpeed(0.005f);
}

CameraController::Type TrackballController::type() const
{
    return Trackball;
}

void TrackballController::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    auto [dx, dy] = event->position() - m_last_mouse_pos;
    if (event->buttons() & Qt::LeftButton) {
        this->updateCameraYaw(-dx * m_sensitivity);
        this->updateCameraPitch(-dy * m_sensitivity);
    }
    if (event->buttons() & Qt::RightButton) {
        m_position += m_right * -dx * m_move_speed;
        m_position += m_up * dy * m_move_speed;
    }
    m_last_mouse_pos = event->position();
}

void TrackballController::processWheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() * m_scale_speed;
    m_position += this->front() * delta;
}

void TrackballController::updateCameraPitch(double angle_deg)
{
    QMatrix4x4 rot_matrix;
    rot_matrix.rotate(angle_deg, m_right);
    m_up = QVector3D(rot_matrix * m_up.toVector4D());
    m_position = QVector3D(rot_matrix * QVector4D(m_position, 1.0f));
}

void TrackballController::updateCameraYaw(double angle_deg)
{
    QMatrix4x4 rot_matrix;
    rot_matrix.rotate(angle_deg, 0.0f, 1.0f, 0.0f);
    m_up = QVector3D(rot_matrix * m_up.toVector4D());
    m_right = QVector3D(rot_matrix * m_right.toVector4D());
    m_position = QVector3D(rot_matrix * QVector4D(m_position, 1.0f));
}
