#include "SpotLight.h"

lcf::SpotLight::SharedPtr lcf::SpotLight::createShared()
{
    return std::make_shared<lcf::SpotLight>();
}

lcf::LightType lcf::SpotLight::lightType() const
{
    return LightType::Spot;
}

lcf::UniformList lcf::SpotLight::asUniformList()
{
    UniformList uniform_list = Light::asUniformList();
    uniform_list.emplace_back(SingleUniform(uniformName("position"), [this] { return this->localPosition(); }));
    uniform_list.emplace_back(SingleUniform(uniformName("direction"), [this] { return this->direction(); }));
    uniform_list.emplace_back(SingleUniform(uniformName("cos_inner"), [this] { return this->m_cos_inner; }));
    uniform_list.emplace_back(SingleUniform(uniformName("cos_outer"), [this] { return this->m_cos_outer; }));
    return uniform_list;
}

void lcf::SpotLight::setInnerAngle(float angle_deg)
{
    m_cos_inner = qCos(qDegreesToRadians(angle_deg));
}

void lcf::SpotLight::setOuterAngle(float angle_deg)
{
    m_cos_outer = qCos(qDegreesToRadians(angle_deg));
}
