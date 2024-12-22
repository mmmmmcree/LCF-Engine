#include "GameController.h"

lcf::GameController::GameController()
{
    this->setSensitivity(0.15f);
    this->setMoveSpeed(0.1f);
}

void lcf::GameController::update(Camera *camera)
{
    this->updateCameraPitch(camera, m_delta_pitch);
    this->updateCameraYaw(camera, m_delta_yaw);
    m_delta_pitch = m_delta_yaw = 0.0f;
    Vector3D direction;
    const Vector3D &front = camera->front();
    const Vector3D &up = camera->up();
    const Vector3D &right = camera->right();
    if (m_direction & Forward) { direction += front; }
    if (m_direction & Backward) { direction -= front; }
    if (m_direction & Left) { direction -= right; }
    if (m_direction & Right) { direction += right; }
    if (m_direction & Up) { direction += up; }
    if (m_direction & Down) { direction -= up; }
    camera->translate(direction.normalized() * m_move_speed);
}

void lcf::GameController::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    auto [dx, dy] = event->position() - m_last_mouse_pos;
    if (event->buttons() & Qt::RightButton) {
        m_delta_yaw -= dx * m_sensitivity;
        m_delta_pitch -= dy * m_sensitivity;
    }
    m_last_mouse_pos = event->position();
}

void lcf::GameController::processKeyPressEvent(QKeyEvent *event)
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

void lcf::GameController::processKeyReleaseEvent(QKeyEvent *event)
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

void lcf::GameController::updateCameraPitch(Camera *camera, double angle_deg)
{
    float temp_pitch = m_pitch + angle_deg;
    if (temp_pitch > 89.0f or temp_pitch < -89.0f) { return; }
    m_pitch = temp_pitch;
    QMatrix4x4 rot_matrix;
    rot_matrix.rotate(angle_deg, camera->right());
    camera->up() = QVector3D(rot_matrix * camera->up().toVector4D());
}

void lcf::GameController::updateCameraYaw(Camera *camera, double angle_deg)
{
    QMatrix4x4 rot_matrix;
    rot_matrix.rotate(angle_deg, 0.0f, 1.0f, 0.0f);
    camera->up() = QVector3D(rot_matrix * camera->up().toVector4D());
    camera->right() = QVector3D(rot_matrix * camera->right().toVector4D());
}
