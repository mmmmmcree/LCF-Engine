#include "SpotLight.h"

lcf::SpotLight::SharedPtr lcf::SpotLight::createShared()
{
    return std::make_shared<lcf::SpotLight>();
}

lcf::LightType lcf::SpotLight::lightType() const
{
    return LightType::Spot;
}

void lcf::SpotLight::setName(std::string_view name)
{
    Light::setName(name);
    m_cos_inner.setName(this->uniformName("cos_inner"));
    m_cos_outer.setName(this->uniformName("cos_outer"));
}

void lcf::SpotLight::setInnerAngle(float angle_deg)
{
    m_cos_inner.setValue(qCos(qDegreesToRadians(angle_deg)));
}

void lcf::SpotLight::setOuterAngle(float angle_deg)
{
    m_cos_outer.setValue(qCos(qDegreesToRadians(angle_deg)));
}
