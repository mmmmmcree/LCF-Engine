#pragma once

#include "NativeTextureWrapper.h"
#include <memory>
#include <vector>
#include <stack>
#include <array>

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
        FrameBufferObject(const FrameBufferObject &) = delete;
        virtual ~FrameBufferObject();
        void checkStatus();
        void bind();
        void blitTo(FrameBufferObject *target, AttachmentType attachment_types, GLTextureFilter color_filter = GLTextureFilter::Nearest);
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
        void setMipMapLevel(int level);
    protected:
        void _bind();
        void _release();
    private:
        using Viewport = std::array<int, 4>;
        using ViewportStack = std::stack<Viewport>;
        using FBOStack = std::stack<unsigned int>;
        inline static ViewportStack s_viewport_stack;
        inline static FBOStack s_fbo_stack;
    protected:
        unsigned int m_fbo;
        int m_width, m_height;
        int m_mip_map_level = 0;
        AttachmentList m_color_attachments;
        NativeTextureWrapper m_depth_attachment;
        NativeTextureWrapper m_depth_stencil_attachment;
    };
}