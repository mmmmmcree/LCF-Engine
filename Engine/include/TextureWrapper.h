#pragma once

#include "GLTexture.h"
#include "ShaderToy.h"
#include "NativeTextureWrapper.h"
#include <optional>


namespace lcf {
    class TextureWrapper
    {
        using _Texture = std::variant<GLTexture *, std::shared_ptr<GLTexture>, ShaderToy *, NativeTextureWrapper>;
    public:
        ~TextureWrapper() = default;
        TextureWrapper() = default;
        TextureWrapper(GLTexture *texture);
        TextureWrapper(ShaderToy *texture);
        TextureWrapper(const std::shared_ptr<GLTexture> &texture);
        TextureWrapper(std::unique_ptr<GLTexture> &&texture);
        TextureWrapper(unsigned int texture);
        TextureWrapper(const NativeTextureWrapper &texture);
        TextureWrapper(const TextureWrapper &other) = default;
        TextureWrapper &operator=(const TextureWrapper &other) = default;
        void bind(unsigned int unit = 0);
        void release(unsigned int unit = 0);
        bool isValid() const;
    private:
        std::optional<_Texture> m_texture;
    };
}