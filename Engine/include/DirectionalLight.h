#pragma once

#include "Light.h"
#include "FrameBufferObject.h"

namespace lcf {
    class DirectionalLight : public Light
    {
    public: 
        using SharedPtr = std::shared_ptr<DirectionalLight>;
        DirectionalLight();
        void bind(int light_index) override;
        void release() override;
        void bindAsShadowMap(int texture_unit) override;
        UniformList asUniformList() override;
        FrameBufferObject::UniquePtr &fbo();
    private:
        FrameBufferObject::UniquePtr m_fbo;
        Matrix4x4 m_light_matrix;
        inline static unsigned int s_ssbo = 0;
        inline static size_t s_ssbo_size = 0;
        inline static size_t s_light_count = 0;
    };
}