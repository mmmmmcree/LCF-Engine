#include "TrackBallController.h"

lcf::TrackballController::TrackballController() : CameraController()
{
    this->setSensitivity(0.2f);
    this->setMoveSpeed(0.005f);
    this->setScaleSpeed(0.005f);
}

void lcf::TrackballController::update(Camera *camera)
{
    this->updateCameraYaw(camera, m_delta_yaw);
    this->updateCameraPitch(camera, m_delta_pitch);
    camera->translate(camera->right() * m_delta_right + camera->up() * m_delta_up + camera->front() * m_delta_front);
    m_delta_yaw = m_delta_pitch = m_delta_up = m_delta_right = m_delta_front = 0.0f;
}

void lcf::TrackballController::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    auto [dx, dy] = event->position() - m_last_mouse_pos;
    if (event->buttons() & Qt::LeftButton) {
        m_delta_yaw -= dx * m_sensitivity;
        m_delta_pitch -= dy * m_sensitivity;
    }
    if (event->buttons() & Qt::RightButton) {
        m_delta_right -= dx * m_move_speed;
        m_delta_up += dy * m_move_speed;
    }
    m_last_mouse_pos = event->position();
}

void lcf::TrackballController::processWheelEvent(QWheelEvent *event)
{
    float delta = event->angleDelta().y() * m_scale_speed;
    m_delta_front += delta;
}

void lcf::TrackballController::updateCameraPitch(Camera *camera, double angle_deg)
{
    Matrix4x4 rot_matrix;
    rot_matrix.rotate(angle_deg, camera->right());
    camera->up() = Vector3D(rot_matrix * camera->up().toVector4D());
    camera->setTranslation(Vector3D(rot_matrix * Vector4D(camera->localPosition(), 1.0f)));
}

void lcf::TrackballController::updateCameraYaw(Camera *camera, double angle_deg)
{
    Matrix4x4 rot_matrix;
    rot_matrix.rotate(angle_deg, 0.0f, 1.0f, 0.0f);
    camera->up() = Vector3D(rot_matrix * camera->up().toVector4D());
    camera->right() = Vector3D(rot_matrix * camera->right().toVector4D());
    camera->setTranslation(Vector3D(rot_matrix * Vector4D(camera->localPosition(), 1.0f)));
}
