#pragma once

#include "Define.h"

namespace lcf {
    class NativeTextureWrapper
    {
    public:
        NativeTextureWrapper() = default;
        NativeTextureWrapper(unsigned int texture);
        NativeTextureWrapper(const NativeTextureWrapper&) = default;
        NativeTextureWrapper *operator->();
        NativeTextureWrapper& operator=(const NativeTextureWrapper&) = default;
        void setTarget(int target);
        void bind(int unit) const;
        void release() const;
        unsigned int texture() const;
    private:
        unsigned int m_texture = 0;
        int m_target = TEXTURE_2D;
    };
}