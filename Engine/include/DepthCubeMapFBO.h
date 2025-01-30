#pragma once

#include "FrameBufferObject.h"

namespace lcf {
    class DepthCubeMapFBO : public FrameBufferObject
    {
    public:
        using UniquePtr = std::unique_ptr<DepthCubeMapFBO>;
        static UniquePtr createUnique(int width);
        DepthCubeMapFBO(int width);
        void resize(int width);
    private:
        void update();
    private:;
        using FrameBufferObject::addColorAttachment;
        using FrameBufferObject::setDepthStencilAttachment;
        using FrameBufferObject::colorAttachment;
        using FrameBufferObject::depthStencilAttachment;
    };
}
