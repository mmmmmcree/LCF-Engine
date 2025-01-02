#pragma once

#include "FrameBufferObject.h"
#include "Define.h"

namespace lcf {
    class SingleColorAttachmentFBO : public FrameBufferObject
    {
    public:
        using SharedPtr = std::shared_ptr<SingleColorAttachmentFBO>;
        static SharedPtr createShared(int width, int height, GLTextureFormat color_format);
        SingleColorAttachmentFBO(int width, int height, GLTextureFormat color_format);
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