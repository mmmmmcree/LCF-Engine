#pragma once

#include "Define.h"
#include <atomic>

namespace lcf {
    class NativeTextureWrapper
    {
    public:
        NativeTextureWrapper();
        ~NativeTextureWrapper();
        NativeTextureWrapper(unsigned int texture);
        NativeTextureWrapper(const NativeTextureWrapper &other);
        NativeTextureWrapper *operator->();
        NativeTextureWrapper& operator=(const NativeTextureWrapper &other);
        void setTarget(int target);
        int target() const;
        void bind(int unit) const;
        void release(int unit) const;
        unsigned int id() const;
    private:
        unsigned int m_texture = 0;
        int m_target = TEXTURE_2D;
        std::atomic<int> *m_ref_count = nullptr;
    };
}