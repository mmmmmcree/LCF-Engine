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
        using TextureList = std::vector<TextureWrapper>;
        void bind() override;
        MaterialType type() const override;
        void setDiffuseMap(TextureWrapper texture);
        void setSpecularMap(TextureWrapper texture);
        void setNormalMap(TextureWrapper texture);
        void setShininess(float shininess);
    private:
        PhongMaterial();
    private:
        TextureList m_textures;
        float m_shininess = 32.0f;
    };
}