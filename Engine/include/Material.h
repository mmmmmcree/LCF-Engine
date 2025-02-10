#pragma once

#include "Uniform.h"
#include <memory>
#include "Define.h"
#include "TextureWrapper.h"

namespace lcf {
    class MaterialController;

    class Material
    {
        friend class MaterialController;
    public:
        using UniquePtr = std::unique_ptr<Material>;
        using SharedPtr = std::shared_ptr<Material>;
        using TextureMap = std::unordered_map<std::string, TextureWrapper>;
        virtual ~Material() = default;
        virtual void bind();
        virtual MaterialType type() const;
        const UniformList &asUniformList() const;
    protected:
        Material() = default;
        const std::string &fromTextureTypeToUniformName(TextureType type) const;
    protected:
        UniformList m_uniforms;
        TextureMap m_texture_map;
        int m_start_location = 0;
    };
}