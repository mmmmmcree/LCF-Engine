#include "Bone.h"

lcf::Bone::Bone(const Bone &other) :
    Transformer(other),
    Hierarchical(other),
    m_name(other.m_name)
{
}

lcf::Bone &lcf::Bone::operator=(const Bone &other)
{
    Transformer::operator=(other);
    Hierarchical::operator=(other);
    m_name = other.m_name;
    return *this;
}

const lcf::Matrix4x4 &lcf::Bone::getWorldMatrix()
{
    if (not m_need_update) { return m_hierarchial_matrix; }
    return m_hierarchial_matrix =
        m_parent ? m_parent->getWorldMatrix() * this->getMatrix() : this->getMatrix();
}

void lcf::Bone::attachTo(Bone *parent)
{
    if (parent == this) { return; }
    m_need_update = UpdateType::All;
    this->_setParent(parent);
}

const lcf::Bone::Children &lcf::Bone::getChildren() const
{
    return m_children;
}

void lcf::Bone::setName(std::string_view name)
{
    m_name = name;
}

const std::string &lcf::Bone::getName() const
{
    return m_name;
}

lcf::Bone *lcf::Bone::getRoot() const
{
    return this->_getRoot();
}

void lcf::Bone::requireUpdate(UpdateType type)
{
    if (m_need_update & type) { return; }
    m_need_update |= type;
    for (auto child : m_children) {
        child->requireUpdate(type);
    }
}
