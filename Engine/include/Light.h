#pragma once

#include "Obejct3D.h"
#include "Uniform.h"

namespace lcf {
    class Light : public Object3D
    {
    public:   
        using SharedPtr = std::shared_ptr<Light>;
        Light() = default;
        virtual UniformList asUniformList() const;
        void setColor(const Vector3D& color);
        void setDiffuseIntensity(float intensity);
        void setSpecularIntensity(float intensity);
        void setAmbientIntensity(float intensity);
    protected:
        std::string uniformName(const std::string &name) const;
    protected:
        Vector3D m_color = {1.0f, 1.0f, 1.0f};
        float m_diffuse_intensity = 1.0f;
        float m_specular_intensity = 0.5f;
        float m_ambient_intensity = 0.2f;
    };
}