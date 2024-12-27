#pragma once

#include "Uniform.h"
#include <memory>
#include "Define.h"
#include "TextureWrapper.h"

namespace lcf {
    class Material
    {
    public:
        using UniquePtr = std::unique_ptr<Material>;
        using SharedPtr = std::shared_ptr<Material>;
        using TextureList = std::vector<TextureWrapper>;
        virtual ~Material() = default;
        virtual void bind();
        virtual void release();
        virtual MaterialType type() const = 0;
        const UniformList &asUniformList() const;
    protected:
        Material() = default;
    protected:
        UniformList m_uniforms;
        TextureList m_textures;
    };
}