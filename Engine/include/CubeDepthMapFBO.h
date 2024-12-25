#pragma once

#include "FrameBufferObject.h"

namespace lcf {
    class CubeDepthMapFBO : public FrameBufferObject
    {
    public:
        using UniquePtr = std::unique_ptr<CubeDepthMapFBO>;
        static UniquePtr createUnique(int width, int height);
        CubeDepthMapFBO(int width, int height);
        void resize(int width, int height);
    private:
        void update();
    private:;
        using FrameBufferObject::addColorAttachment;
        using FrameBufferObject::setDepthStencilAttachment;
        using FrameBufferObject::colorAttachment;
        using FrameBufferObject::depthStencilAttachment;
    };
}
