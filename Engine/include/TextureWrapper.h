#pragma once

#include "GLTexture.h"
#include "NativeTextureWrapper.h"
#include <optional>


namespace lcf {
    class TextureWrapper
    {
        using _Texture = std::variant<GLTexture *, std::shared_ptr<GLTexture>, NativeTextureWrapper>;
    public:
        ~TextureWrapper() = default;
        TextureWrapper() = default;
        TextureWrapper(GLTexture *texture);
        TextureWrapper(const std::shared_ptr<GLTexture> &texture);
        TextureWrapper(std::unique_ptr<GLTexture> &&texture);
        TextureWrapper(const NativeTextureWrapper &texture);
        TextureWrapper(const TextureWrapper &other) = default;
        TextureWrapper &operator=(const TextureWrapper &other) = default;
        TextureWrapper *operator->();
        void bind(unsigned int unit = 0);
        void release(unsigned int unit = 0);
        bool isValid();
        int width();
        int height();
        int depth();
        GLTexture::Target target();
        GLTexture::TextureFormat format();
        void setWrapMode(GLTexture::WrapMode wrap_mode);
        void setMinMagFilters(GLTexture::Filter min_filter, GLTexture::Filter mag_filter);
        void setBorderColor(float r, float g, float b, float a);
        void generateMipMaps();
    private:
        std::optional<_Texture> m_texture;
    };
}