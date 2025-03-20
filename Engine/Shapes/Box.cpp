#include "Box.h"
#include "my_math.h"

lcf::Box::Box()
{
    float inf = std::numeric_limits<float>::max();
    m_min_position = {inf, inf, inf};
    m_max_position = -m_min_position;
}

lcf::Box::Box(const Vector3D &min_position, const Vector3D &max_position) :
    BaseConstructor(),
    m_min_position(min_position),
    m_max_position(max_position)
{
}

lcf::Box::Box(std::span<Vector3D> positions) :
    Box()
{
    for (const auto &position : positions) {
        m_min_position = lcf::min(m_min_position, position);
        m_max_position = lcf::max(m_max_position, position);
    }
}

lcf::Vector3D lcf::Box::getCenter() const
{
    return (m_min_position + m_max_position) / 2.0f;
}

float lcf::Box::getWidth() const
{
    return m_max_position.x() - m_min_position.x();
}

float lcf::Box::getHeight() const
{
    return m_max_position.y() - m_min_position.y();
}

float lcf::Box::getDepth() const
{
    return m_max_position.z() - m_min_position.z();
}

lcf::Vector3D lcf::Box::getDimensions() const
{
    return m_max_position - m_min_position;
}

lcf::Box lcf::operator+(const Box &lhs, const Box &rhs)
{
    return Box(lcf::min(lhs.m_min_position, rhs.m_min_position), lcf::max(lhs.m_max_position, rhs.m_max_position));
}

lcf::Box &lcf::operator+=(Box &lhs, const Box &rhs)
{
    lhs.m_min_position = lcf::min(lhs.m_min_position, rhs.m_min_position);
    lhs.m_max_position = lcf::max(lhs.m_max_position, rhs.m_max_position);
    return lhs;
}
