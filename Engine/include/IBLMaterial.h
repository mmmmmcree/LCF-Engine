#pragma once

#include "Material.h"
#include "TextureWrapper.h"
#include <unordered_map>

namespace lcf {
    class MaterialController;
    
    class IBLMaterial : public Material 
    {
        friend class MaterialController;
    public:
        using UniquePtr = std::unique_ptr<IBLMaterial>;
        using TextureMap = std::unordered_map<std::string, TextureWrapper>;
        MaterialType type() const override;
        void setIrradianceMap(TextureWrapper texture);
        void setPrefilterMap(TextureWrapper texture);
        void setBRDFMap(TextureWrapper texture);
    private:
        static UniquePtr createUnique();
        IBLMaterial();
    };
}