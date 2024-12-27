#pragma once

#include "Material.h"
#include "TextureWrapper.h"

namespace lcf {
    class MaterialController;

    class PhongMaterial : public Material
    {
        friend class MaterialController;
    public:
        using UniquePtr = std::unique_ptr<PhongMaterial>;
        MaterialType type() const override;
        void setDiffuseMap(TextureWrapper texture);
        void setSpecularMap(TextureWrapper texture);
        void setNormalMap(TextureWrapper texture);
        void setShininess(float shininess);
    private:
        PhongMaterial();
    private:
        float m_shininess = 32.0f;
    };
}