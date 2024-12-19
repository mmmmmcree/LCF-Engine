#include "PointLight.h"

lcf::UniformList lcf::PointLight::asUniformList()
{
    UniformList uniform_list = Light::asUniformList();
    uniform_list.emplace_back(SingleUniform(uniformName("position"), [this] { return this->position(); }));
    uniform_list.emplace_back(SingleUniform(uniformName("constant"), [this] { return this->m_constant; }));
    uniform_list.emplace_back(SingleUniform(uniformName("linear"), [this] { return this->m_linear; }));
    uniform_list.emplace_back(SingleUniform(uniformName("quadratic"), [this] { return this->m_quadratic; }));
    return uniform_list;
}