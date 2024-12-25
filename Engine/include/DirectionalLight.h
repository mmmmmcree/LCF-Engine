#pragma once

#include "Light.h"
#include "DepthMapFBO.h"

namespace lcf {
    class DirectionalLight : public Light
    {
    public: 
        DirectionalLight();
        using SharedPtr = std::shared_ptr<DirectionalLight>;
        static SharedPtr createShared();
        LightType lightType() const override;
        int index() const;
        void bind() override;
        void release() override;
        UniformList asUniformList() override;
        const NativeTextureWrapper &shadowMapTexture() const;
    private:
        DepthMapFBO::UniquePtr m_fbo;
        Matrix4x4 m_light_matrix;
        int m_light_index = 0;
        inline static unsigned int s_ssbo = 0;
        inline static int s_ssbo_size = 0;
        inline static int s_light_count = 0;
    };
}