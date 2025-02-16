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
        using LightList = std::vector<Light *>;
        LightArray();
        void addDirectionalLight(const DirectionalLight::SharedPtr &light);
        const DirectionalLight::SharedPtr &takeDirectionalLight(size_t index) const;
        int directionalLightCount() const;
        void addPointLight(const PointLight::SharedPtr &light);
        const PointLight::SharedPtr &takePointLight(size_t index) const;
        int pointLightCount() const;
        void addSpotLight(const SpotLight::SharedPtr &light);
        const SpotLight::SharedPtr &takeSpotLight(size_t index) const;
        int spotLightCount() const;
        typename LightList::iterator begin();
        typename LightList::iterator end();
        typename LightList::const_iterator begin() const;
        typename LightList::const_iterator end() const;
    private:
        DirectionalLightList m_directional_lights;
        PointLightList m_point_lights;
        SpotLightList m_spot_lights;
        LightList m_lights;
        SingleUniform<int> m_directional_light_count = 0;
        SingleUniform<int> m_point_light_count = 0;
        SingleUniform<int> m_spot_light_count = 0;
    };
}