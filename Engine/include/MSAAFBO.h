#pragma once

#include "FrameBufferObject.h"

namespace lcf {
    class MSAAFBO : public FrameBufferObject
    {
    public:
        using UniquePtr = std::unique_ptr<MSAAFBO>;
        static UniquePtr createUnique(int width, int height, int samples, GLTextureFormat color_format);
        MSAAFBO(int width, int height, int samples, GLTextureFormat color_format);
        void resize(int width, int height);
        void setSamples(int samples);
        void setColorFormat(GLTextureFormat format);
    private:
        void update();
    private:
        using FrameBufferObject::addColorAttachment;
        using FrameBufferObject::setDepthAttachment;
        using FrameBufferObject::colorAttachment;
        using FrameBufferObject::depthAttachment;
    private:
        int m_samples;
        GLTextureFormat m_color_format;
    };
}