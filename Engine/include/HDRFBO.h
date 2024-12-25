#pragma once

#include "FrameBufferObject.h"

namespace lcf {
    class HDRFBO : public FrameBufferObject
    {
    public:
        using UniquePtr = std::unique_ptr<HDRFBO>;
        static UniquePtr createUnique(int width, int height);
        HDRFBO(int width, int height);
        void resize(int width, int height);
    private:
        void update();
    private:
        using FrameBufferObject::addColorAttachment;
        using FrameBufferObject::setColorAttachment;
        using FrameBufferObject::setDepthAttachment;
        using FrameBufferObject::setDepthStencilAttachment;
    };
}