#pragma once

#include "Light.h"
#include "DepthMapFBO.h"
#include "ShaderStorageBufferObject.h"

namespace lcf {
    class DirectionalLight : public Light
    {
    public: 
        DirectionalLight();
        using SharedPtr = std::shared_ptr<DirectionalLight>;
        static SharedPtr createShared();
        LightType lightType() const override;
        void update() override;
        int index() const;
        void bind() override;
        void release() override;
        void setName(std::string_view name) override;
    protected:
    private:
        DepthMapFBO::UniquePtr m_fbo;
        Matrix4x4 m_light_matrix;
        int m_light_index = 0;
        SingleUniform<float> m_near_plane, m_frustum_size, m_light_size = 1.0f;
        inline static int s_light_count = 0;
        inline static ShaderStorageBufferObject s_ssbo;
    };
}