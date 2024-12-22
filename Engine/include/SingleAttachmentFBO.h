#pragma once

// #include "GLFrameBufferObject.h"
#include "NativeTextureWrapper.h"
#include <memory>

namespace lcf {
    class SingleAttachmentFBO
    {
    public:
        enum AttachmentType
        {
            DepthMap,
            CubeDepthMap,
        };
        using UniquePtr = std::unique_ptr<SingleAttachmentFBO>;
        static UniquePtr createUnique(int width, int height, AttachmentType attachment_type);
        SingleAttachmentFBO(int width, int height, AttachmentType attachment_type);
        ~SingleAttachmentFBO();
        const NativeTextureWrapper &texture() const;
        void bind();
        void release();
        int width() const;
        int height() const;
    private:
        void configureDepthMapAttachment();
        void configureCubeDepthMapAttachment();
    private:
        int m_original_viewport[4];
        unsigned int m_fbo;
        int m_width, m_height;
        NativeTextureWrapper m_texture;
    };
}