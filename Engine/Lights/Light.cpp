#include "Light.h"

lcf::UniformList lcf::Light::asUniformList() const
{
    UniformList uniform_list;
    uniform_list.emplace_back(SingleUniform(uniformName("color"), [this] { return this->m_color; }));
    uniform_list.emplace_back(SingleUniform(uniformName("diffuse_intensity"), [this] { return this->m_diffuse_intensity; }));
    uniform_list.emplace_back(SingleUniform(uniformName("specular_intensity"), [this] { return this->m_specular_intensity; }));
    uniform_list.emplace_back(SingleUniform(uniformName("shininess"), [this] { return this->m_shininess; }));
    uniform_list.emplace_back(SingleUniform(uniformName("ambient_intensity"), [this] { return this->m_ambient_intensity; }));
    return uniform_list;
}

void lcf::Light::setColor(const Vector3D &color)
{
    m_color = color;
}

void lcf::Light::setDiffuseIntensity(float intensity)
{
    m_diffuse_intensity = intensity;
}

void lcf::Light::setSpecularIntensity(float intensity)
{
    m_specular_intensity = intensity;
}

void lcf::Light::setShininess(float shininess)
{
    m_shininess = shininess;
}

void lcf::Light::setAmbientIntensity(float intensity)
{
    m_ambient_intensity = intensity;
}

std::string lcf::Light::uniformName(const std::string &name) const
{
    return this->name() + "." + name;
}