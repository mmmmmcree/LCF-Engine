#include"LightArray.h"

const lcf::DirectionalLight::SharedPtr &lcf::LightArray::addDirectionalLight()
{
    m_directional_lights.push_back(DirectionalLight::createShared());
    auto &light = m_directional_lights.back();
    light->setName("directional_light[" + std::to_string(m_directional_lights.size() -1) + "]");
    return light;
}

const lcf::DirectionalLight::SharedPtr &lcf::LightArray::directionalLight(size_t index) const
{
    return m_directional_lights[index];
}

int lcf::LightArray::directionalLightCount() const
{
    return static_cast<int>(m_directional_lights.size());
}

const lcf::PointLight::SharedPtr &lcf::LightArray::addPointLight()
{
    m_point_lights.push_back(PointLight::createShared());
    auto &light = m_point_lights.back();
    light->setName("point_light[" + std::to_string(m_point_lights.size() - 1) + "]");
    return light;
}

const lcf::PointLight::SharedPtr &lcf::LightArray::pointLight(size_t index) const
{
    return m_point_lights[index];
}

int lcf::LightArray::pointLightCount() const
{
    return static_cast<int>(m_point_lights.size());
}

const lcf::SpotLight::SharedPtr &lcf::LightArray::addSpotLight()
{
    m_spot_lights.push_back(SpotLight::createShared());
    auto &light = m_spot_lights.back();
    light->setName("spot_light[" + std::to_string(m_spot_lights.size() - 1) + "]");
    return light;
}

const lcf::SpotLight::SharedPtr &lcf::LightArray::spotLight(size_t index) const
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
    for (auto& light : m_directional_lights) {
        const auto& light_uniforms = light->asUniformList();
        uniforms.insert(uniforms.end(), light_uniforms.begin(), light_uniforms.end());
    }
    for (auto& light : m_point_lights) {
        const auto& light_uniforms = light->asUniformList();
        uniforms.insert(uniforms.end(), light_uniforms.begin(), light_uniforms.end());
    }
    for (auto& light : m_spot_lights) {
        const auto& light_uniforms = light->asUniformList();
        uniforms.insert(uniforms.end(), light_uniforms.begin(), light_uniforms.end());
    }
    return uniforms;
}

lcf::LightArray::LightList lcf::LightArray::all() const
{
    LightList lights;
    lights.insert(lights.end(), m_directional_lights.begin(), m_directional_lights.end());
    lights.insert(lights.end(), m_point_lights.begin(), m_point_lights.end());
    lights.insert(lights.end(), m_spot_lights.begin(), m_spot_lights.end());
    return lights;
}
