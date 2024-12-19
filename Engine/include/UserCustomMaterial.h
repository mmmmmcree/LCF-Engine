#pragma once

#include "Material.h"
#include "Define.h"
#include "TextureWrapper.h"

namespace lcf {
    class UserCustomMaterial : public Material
    {
    public:
        using SharedPtr = std::shared_ptr<UserCustomMaterial>;
        using UniquePtr = std::unique_ptr<UserCustomMaterial>;
        using TextureInfoMap = std::map<int, TextureWrapper>;
        void bind() override;
        MaterialType type() const override;
        void setTexture(TextureType texture_type, TextureWrapper texture);
    private: 
        TextureInfoMap m_textures;
    };
}