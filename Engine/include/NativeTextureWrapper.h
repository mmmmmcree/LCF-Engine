#pragma once

namespace lcf {
    class NativeTextureWrapper
    {
    public:
        NativeTextureWrapper(unsigned int texture);
        NativeTextureWrapper(const NativeTextureWrapper&) = default;
        NativeTextureWrapper *operator->();
        void bind(int unit) const;
        void release() const;
    private:
        unsigned int m_texture;
    };
}