#pragma once

#include "MMaterial.h"
#include "TextureWrapper.h"

namespace lcf {
    class PhongMaterial : public MMaterial
    {
    public:
        using UniquePtr = std::unique_ptr<PhongMaterial>;
        using TextureList = std::vector<TextureWrapper>;
        PhongMaterial();
        void bind() override;
        void setDiffuseMap(TextureWrapper texture);
        void setSpecularMap(TextureWrapper texture);
        void setNormalMap(TextureWrapper texture);
        void setShininess(float shininess);
    private:
        TextureList m_textures;
        float m_shininess = 32.0f;
    };
}