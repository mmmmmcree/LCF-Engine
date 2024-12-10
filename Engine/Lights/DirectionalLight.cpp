#include "DirectionalLight.h"

lcf::UniformList lcf::DirectionalLight::asUniformList() const
{
    UniformList uniform_list = Light::asUniformList();
    uniform_list.emplace_back(SingleUniform(uniformName("direction"), [this] { return this->m_direction; }));
    return uniform_list;
}

void lcf::DirectionalLight::setDirection(const Vector3D &direction)
{
    m_direction = direction;
}
