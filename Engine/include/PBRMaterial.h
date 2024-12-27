#pragma once

#include "Material.h"
#include "TextureWrapper.h"

namespace lcf {
    class MaterialController;

    class PBRMaterial : public Material
    {
        friend class MaterialController;
    public:
        using UniquePtr = std::unique_ptr<PBRMaterial>;
        MaterialType type() const override;
        void setAlbedoMap(TextureWrapper texture);
        void setNormalMap(TextureWrapper texture);
        void setMetallicMap(TextureWrapper texture);
        void setRoughnessMap(TextureWrapper texture);
        void setAOMap(TextureWrapper texture);
        void setEmissiveMap(TextureWrapper texture);
    private:
        PBRMaterial();
    };
}