#pragma once

#include "Light.h"
#include "SingleAttachmentFBO.h"

namespace lcf {
    class DirectionalLight : public Light
    {
    public: 
        DirectionalLight();
        using SharedPtr = std::shared_ptr<DirectionalLight>;
        static SharedPtr createShared();
        LightType lightType() const override;
        void bind() override;
        void release() override;
        void bindAsShadowMap(int texture_unit) override;
        UniformList asUniformList() override;
        SingleAttachmentFBO::UniquePtr &fbo();
    private:
        SingleAttachmentFBO::UniquePtr m_fbo;
        Matrix4x4 m_light_matrix;
        int m_light_index = 0;
        inline static unsigned int s_ssbo = 0;
        inline static int s_ssbo_size = 0;
        inline static int s_light_count = 0;
    };
}