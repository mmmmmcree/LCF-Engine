#pragma once

#include "FrameBufferObject.h"

namespace lcf {
    class ScreenFBO : public FrameBufferObject
    {
    public:
        using UniquePtr = std::unique_ptr<ScreenFBO>;
        static UniquePtr createUnique(int width, int height, GLTextureFormat internal_fmt);
        ScreenFBO(int width, int height, GLTextureFormat color_format);
        void resize(int width, int height);
        void setColorFormat(GLTextureFormat color_format);
    private:
        void update();
    private:
        using FrameBufferObject::addColorAttachment;
        using FrameBufferObject::setColorAttachment;
        using FrameBufferObject::setDepthAttachment;
        using FrameBufferObject::depthAttachment;
    private:
        GLTextureFormat m_color_format;
    };
}