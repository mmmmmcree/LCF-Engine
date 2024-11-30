#include "GameController.h"

GameController::GameController()
{
    this->reset();
    this->setSensitivity(0.15f);
    this->setMoveSpeed(0.1f);
}

void GameController::update()
{
    QVector3D direction, front = this->front();
    if (m_direction & Forward) { direction += front; }
    if (m_direction & Backward) { direction -= front; }
    if (m_direction & Left) { direction -= m_right; }
    if (m_direction & Right) { direction += m_right; }
    if (m_direction & Up) { direction += m_up; }
    if (m_direction & Down) { direction -= m_up; }
    m_position += direction.normalized() * m_move_speed;
}

CameraController::Type GameController::type() const
{
    return Type::Game;
}

void GameController::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    auto [dx, dy] = event->position() - m_last_mouse_pos;
    if (event->buttons() & Qt::RightButton) {
        this->updateCameraYaw(-dx * m_sensitivity);
        this->updateCameraPitch(-dy * m_sensitivity);
    }
    m_last_mouse_pos = event->position();
}

void GameController::processKeyPressEvent(QKeyEvent *event)
{
    CameraController::processKeyPressEvent(event);
    switch(event->key()) {
        case Qt::Key_W: { m_direction |= Forward; } break;
        case Qt::Key_S: { m_direction |= Backward; } break;
        case Qt::Key_A: { m_direction |= Left; } break;
        case Qt::Key_D: { m_direction |= Right; } break;
        case Qt::Key_Q: { m_direction |= Up; } break;
        case Qt::Key_E: { m_direction |= Down; } break;
    }
}

void GameController::processKeyReleaseEvent(QKeyEvent *event)
{
    CameraController::processKeyReleaseEvent(event);
    switch(event->key()) {
        case Qt::Key_W: { m_direction &= ~Forward; } break;
        case Qt::Key_S: { m_direction &= ~Backward; } break;
        case Qt::Key_A: { m_direction &= ~Left; } break;
        case Qt::Key_D: { m_direction &= ~Right; } break;
        case Qt::Key_Q: { m_direction &= ~Up; } break;
        case Qt::Key_E: { m_direction &= ~Down; } break;
    }
}

void GameController::updateCameraPitch(double angle_deg)
{
    float temp_pitch = m_pitch + angle_deg;
    if (temp_pitch > 89.0f or temp_pitch < -89.0f) { return; }
    m_pitch = temp_pitch;
    QMatrix4x4 rot_matrix;
    rot_matrix.rotate(angle_deg, m_right);
    m_up = QVector3D(rot_matrix * m_up.toVector4D());
}

void GameController::updateCameraYaw(double angle_deg)
{
    QMatrix4x4 rot_matrix;
    rot_matrix.rotate(angle_deg, 0.0f, 1.0f, 0.0f);
    m_up = QVector3D(rot_matrix * m_up.toVector4D());
    m_right = QVector3D(rot_matrix * m_right.toVector4D());
}
