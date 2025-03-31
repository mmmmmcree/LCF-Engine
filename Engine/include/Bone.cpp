#include "Bone.h"

void lcf::Bone::setName(std::string_view name)
{
    m_name = name;
}

const std::string &lcf::Bone::getName() const
{
    return m_name;
}