#pragma once

#include "NativeTextureWrapper.h"
#include <memory>
#include <vector>

namespace lcf {
    class FrameBufferObject
    {
    public:
        enum AttachmentType
        {
            ColorAttachment = 1,
            DepthAttachment = 1 << 1,
            DepthStencilAttachment = 1 << 2
        };
        using UniquePtr = std::unique_ptr<FrameBufferObject>;
        using AttachmentList = std::vector<NativeTextureWrapper>;
        static UniquePtr createUnique(int width, int height);
        FrameBufferObject(int width, int height);
        virtual ~FrameBufferObject();
        void checkStatus();
        void bind();
        void blitTo(FrameBufferObject *target, AttachmentType attachment_types);
        void release();
        int width() const;
        int height() const;
        unsigned int id() const;
        void addColorAttachment();
        void addColorAttachment(const NativeTextureWrapper &color_attachment);
        void setColorAttachment(int index, const NativeTextureWrapper &color_attachment);
        void setDepthAttachment(const NativeTextureWrapper &depth_attachment);
        void setDepthStencilAttachment(const NativeTextureWrapper &depth_stencil_attachment);
        const NativeTextureWrapper &colorAttachment(int index = 0) const;
        const NativeTextureWrapper &depthAttachment() const;
        const NativeTextureWrapper &depthStencilAttachment() const;
    private:
        int m_original_viewport[4];
        int m_last_bound_fbo;
    protected:
        unsigned int m_fbo;
        int m_width, m_height;
        AttachmentList m_color_attachments;
        NativeTextureWrapper m_depth_attachment;
        NativeTextureWrapper m_depth_stencil_attachment;
    };
}