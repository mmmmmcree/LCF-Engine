#include"LightArray.h"
#include "GLHelper.h"

void lcf::LightArray::addDirectionalLight(const DirectionalLight::SharedPtr &light)
{
    m_directional_lights.push_back(light);
    light->setName("directional_light[" + std::to_string(m_directional_lights.size() - 1) + "]");
    m_lights.push_back(light.get());
}

const lcf::DirectionalLight::SharedPtr &lcf::LightArray::takeDirectionalLight(size_t index) const
{
    return m_directional_lights[index];
}

int lcf::LightArray::directionalLightCount() const
{
    return static_cast<int>(m_directional_lights.size());
}

void lcf::LightArray::addPointLight(const PointLight::SharedPtr &light)
{
    m_point_lights.push_back(light);
    light->setName("point_light[" + std::to_string(m_point_lights.size() - 1) + "]");
    m_lights.push_back(light.get());
}

const lcf::PointLight::SharedPtr &lcf::LightArray::takePointLight(size_t index) const
{
    return m_point_lights[index];
}

int lcf::LightArray::pointLightCount() const
{
    return static_cast<int>(m_point_lights.size());
}

void lcf::LightArray::addSpotLight(const SpotLight::SharedPtr &light)
{
    m_spot_lights.push_back(light);
    light->setName("spot_light[" + std::to_string(m_spot_lights.size() - 1) + "]");
    m_lights.push_back(light.get());
}

const lcf::SpotLight::SharedPtr &lcf::LightArray::takeSpotLight(size_t index) const
{
    return m_spot_lights[index];
}

int lcf::LightArray::spotLightCount() const
{
    return static_cast<int>(m_spot_lights.size());
}

lcf::UniformList lcf::LightArray::asUniformList()
{
    UniformList uniforms;
    for (Light *light : m_lights) {
        const auto& light_uniforms = light->asUniformList();
        uniforms.insert(uniforms.end(), light_uniforms.begin(), light_uniforms.end());
    }
    uniforms.push_back(SingleUniform("directional_light_count", [this] { return this->directionalLightCount(); }));
    uniforms.push_back(SingleUniform("point_light_count", [this] { return this->pointLightCount(); }));
    uniforms.push_back(SingleUniform("spot_light_count", [this] { return this->spotLightCount(); }));
    return uniforms;
}

typename lcf::LightArray::LightList::iterator lcf::LightArray::begin()
{
    return m_lights.begin();
}

typename lcf::LightArray::LightList::iterator lcf::LightArray::end()
{
    return m_lights.end();
}

typename lcf::LightArray::LightList::const_iterator lcf::LightArray::begin() const
{
    return m_lights.begin();
}

typename lcf::LightArray::LightList::const_iterator lcf::LightArray::end() const
{
    return m_lights.end();
}
