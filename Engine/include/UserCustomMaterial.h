#pragma once

#include "Material.h"
#include "Define.h"
#include "TextureWrapper.h"

namespace lcf {
    class MaterialController;

    class UserCustomMaterial : public Material
    {
        friend class MaterialController;
    public:
        using SharedPtr = std::shared_ptr<UserCustomMaterial>;
        using UniquePtr = std::unique_ptr<UserCustomMaterial>;
        using TextureInfoMap = std::map<int, TextureWrapper>;
        void dispatch() override;
        MaterialType type() const override;
    private:
        UserCustomMaterial(TextureInfoMap *textures);
    private: 
        TextureInfoMap *m_textures;
    };
}