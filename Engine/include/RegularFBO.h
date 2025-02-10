#pragma once

#include "FrameBufferObject.h"
#include <functional>
#include "GLHelper.h"

namespace lcf {
    class RegularFBO : public FrameBufferObject
    {
    public:
        using UniquePtr = std::unique_ptr<RegularFBO>;
        static UniquePtr createUnique(int width, int height);
        RegularFBO(int width, int height);
        void resize(int width, int height);
    private:
        void update();
    private:
        using FrameBufferObject::addColorAttachment;
        using FrameBufferObject::setColorAttachment;
        using FrameBufferObject::setDepthAttachment;
        using FrameBufferObject::setDepthStencilAttachment;
    private:
        std::function<NativeTextureWrapper(int, int)> m_color_attachment_generator = [](int width, int height) {
            return GLHelper::generateTexture2DAttachment(width, height, GLTextureFormat::RGBA8U);
        };
    };
}