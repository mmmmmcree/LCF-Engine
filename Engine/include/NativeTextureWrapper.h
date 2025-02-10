#pragma once

#include "Define.h"
#include <atomic>
#include <utility>
#include "GLTexture.h"

namespace lcf {
    class NativeTextureWrapper
    {
    public:
        NativeTextureWrapper() = default;
        ~NativeTextureWrapper();
        NativeTextureWrapper(GLTexture::Target target, unsigned int texture, bool managed = true);
        NativeTextureWrapper(const NativeTextureWrapper &other);
        NativeTextureWrapper *operator->();
        NativeTextureWrapper& operator=(const NativeTextureWrapper &other);
        GLTexture::Target target() const;
        void bind(int unit) const;
        void release(int unit) const;
        unsigned int textureId() const;
        bool isValid() const;
        std::pair<int, int> size() const;
        int width() const;
        int height() const;
        GLTexture::TextureFormat format() const;
        void setWrapMode(GLTexture::WrapMode wrap_mode);
        void setMinMagFilters(GLTexture::Filter min_filter, GLTexture::Filter mag_filter);
        void setBorderColor(float r, float g, float b, float a);
        void generateMipMaps();
    private:
        struct TextureInfo
        {
            unsigned int texture = 0;
            int target = GLTexture::Target2D;
            int width = 0, height = 0;
            int format = GLTexture::TextureFormat::NoFormat;
            std::atomic<int> m_ref_count = std::numeric_limits<int>::max() >> 1;
        };
        TextureInfo *m_info = nullptr;
    };
}