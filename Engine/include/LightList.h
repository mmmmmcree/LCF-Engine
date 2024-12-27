#pragma once

#include "PointLight.h"
#include <vector>

namespace lcf {
    template <typename LightType>
    class LightList
    {
    public: 
        using SharedLightPtr = typename LightType::SharedPtr;
        using List = std::vector<SharedLightPtr>;
        LightList() = default;
        void addLight(const SharedLightPtr& light);
        UniformList asUniformList();
        // SharedLightPtr &operator[](size_t index);
        const SharedLightPtr &operator[](size_t index) const;
        int size() const;
    private:
        List m_lights;
    };
}

template <typename LightType>
inline void lcf::LightList<LightType>::addLight(const SharedLightPtr &light)
{
    m_lights.push_back(light);
    std::string name;
    if constexpr (std::is_same_v<LightType, PointLight>) {
        name = "point_light[";;
    } else if constexpr (std::is_same_v<LightType, DirectionalLight>) {
        name = "directional_light[";
    } else if constexpr (std::is_same_v<LightType, SpotLight>) {
        name = "spot_light[";
    }
    name += std::to_string(m_lights.size() - 1) + "]";
    light->setName(name);
}

template <typename LightType>
inline lcf::UniformList lcf::LightList<LightType>::asUniformList()
{
    UniformList uniforms;
    for (auto& light : m_lights) {
        const auto& light_uniforms = light->asUniformList();
        uniforms.insert(uniforms.end(), light_uniforms.begin(), light_uniforms.end());
    }
    return uniforms;
}

template <typename LightType>
inline const typename lcf::LightList<LightType>::SharedLightPtr &lcf::LightList<LightType>::operator[](size_t index) const
{
    return m_lights[index];
}

template <typename LightType>
inline int lcf::LightList<LightType>::size() const
{
    return static_cast<int>(m_lights.size());
}
