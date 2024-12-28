#pragma once

#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <vector>

namespace lcf {
    class LightArray
    {
    public: 
        using DirectionalLightList = std::vector<typename DirectionalLight::SharedPtr>;
        using PointLightList = std::vector<typename PointLight::SharedPtr>;
        using SpotLightList = std::vector<typename SpotLight::SharedPtr>;
        using LightList = std::vector<Light::SharedPtr>;
        LightArray() = default;
        const DirectionalLight::SharedPtr &addDirectionalLight();
        const DirectionalLight::SharedPtr &directionalLight(size_t index) const;
        int directionalLightCount() const;
        const PointLight::SharedPtr &addPointLight();
        const PointLight::SharedPtr &pointLight(size_t index) const;
        int pointLightCount() const;
        const SpotLight::SharedPtr &addSpotLight();
        const SpotLight::SharedPtr &spotLight(size_t index) const;
        int spotLightCount() const;
        UniformList asUniformList();
        LightList all() const;
    private:
        DirectionalLightList m_directional_lights;
        PointLightList m_point_lights;
        SpotLightList m_spot_lights;
    };
}