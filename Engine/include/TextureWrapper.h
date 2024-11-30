#pragma once

#include "GLTexture.h"
// #include "ShaderToys/ShaderToy.h"

class ShaderToy;

namespace lcf {
    class TextureWrapper
    {
        using _Texture = std::variant<GLTexture *, std::shared_ptr<GLTexture>, ShaderToy *>;
    public:
        TextureWrapper(GLTexture *texture);
        TextureWrapper(ShaderToy *texture);
        TextureWrapper(const std::shared_ptr<GLTexture> &texture);
        TextureWrapper(std::unique_ptr<GLTexture> &&texture);
        TextureWrapper(const TextureWrapper &other) = default;
        void bind(unsigned int unit = 0);
        void release();
    private:
        _Texture m_texture;
    };
}