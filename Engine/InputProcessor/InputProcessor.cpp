#include "InputProcessor.h"
#include <QCursor>

lcf::InputProcessor *lcf::InputProcessor::instance()
{
    static InputProcessor s_instance;
    return &s_instance;
}

void lcf::InputProcessor::processMouseMoveEvent(QMouseEvent *event)
{
    auto [x, y] = event->position();
    m_accumulated_mouse_offset += Vector2D(x, y) - m_previous_mouse_pos;
    m_previous_mouse_pos = Vector2D(x, y);
    m_buttons = event->buttons();
}

void lcf::InputProcessor::processKeyEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) { return; }
    if (event->type() == QEvent::KeyPress) {
        this->processKeyPressedEvent(event);
    } else {
        this->processKeyReleasedEvent(event);
    }
}

void lcf::InputProcessor::processWheelEvent(QWheelEvent *event)
{
    auto [x, y] = event->angleDelta();
    m_accumulated_wheel_offset += Vector2D(x, y);
}

lcf::Vector2D lcf::InputProcessor::getMouseOffset()
{
    Vector2D offset = m_accumulated_mouse_offset;
    m_accumulated_mouse_offset = Vector2D(0.0f, 0.0f);
    return offset;
}

lcf::Vector2D lcf::InputProcessor::getWheelOffset()
{
    Vector2D offset = m_accumulated_wheel_offset;
    m_accumulated_wheel_offset = Vector2D(0.0f, 0.0f);
    return offset;
}

lcf::Vector3D lcf::InputProcessor::getMovingDirection(const Vector3D &up, const Vector3D &right) const
{
    Vector3D front = Vector3D::crossProduct(up, right);
    Vector3D direction = Vector3D(0.0f, 0.0f, 0.0f);
    if (this->isMovingForward()) { direction += front; }
    if (this->isMovingBackward()) { direction -= front; }
    if (this->isMovingLeft()) { direction -= right; }
    if (this->isMovingRight()) { direction += right; }
    if (this->isMovingUp()) { direction += up; }
    if (this->isMovingDown()) { direction -= up; }
    direction.normalize();
    return direction;
}

bool lcf::InputProcessor::isMovingForward() const
{
    return m_direction & Forward;
}

bool lcf::InputProcessor::isMovingBackward() const
{
    return m_direction & Backward;
}

bool lcf::InputProcessor::isMovingLeft() const
{
    return m_direction & Left;
}

bool lcf::InputProcessor::isMovingRight() const
{
    return m_direction & Right;
}

bool lcf::InputProcessor::isMovingUp() const
{
    return m_direction & Up;
}

bool lcf::InputProcessor::isMovingDown() const
{
    return m_direction & Down;
}

bool lcf::InputProcessor::isLeftMouseButtonPressed() const
{
    return m_buttons & Qt::LeftButton;
}

bool lcf::InputProcessor::isRightMouseButtonPressed() const
{
    return m_buttons & Qt::RightButton;
}

void lcf::InputProcessor::processKeyPressedEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_W: { m_direction |= Forward; } break;
        case Qt::Key_S: { m_direction |= Backward; } break;
        case Qt::Key_A: { m_direction |= Left; } break;
        case Qt::Key_D: { m_direction |= Right; } break;
        case Qt::Key_Q: { m_direction |= Up; } break;
        case Qt::Key_E: { m_direction |= Down; } break;
    }
}

void lcf::InputProcessor::processKeyReleasedEvent(QKeyEvent *event)
{
    switch(event->key()) {
        case Qt::Key_W: { m_direction &= ~Forward; } break;
        case Qt::Key_S: { m_direction &= ~Backward; } break;
        case Qt::Key_A: { m_direction &= ~Left; } break;
        case Qt::Key_D: { m_direction &= ~Right; } break;
        case Qt::Key_Q: { m_direction &= ~Up; } break;
        case Qt::Key_E: { m_direction &= ~Down; } break;
    }
}
