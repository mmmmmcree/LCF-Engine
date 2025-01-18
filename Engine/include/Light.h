#pragma once

#include "Object3D.h"
#include "Uniform.h"
#include "ProjectionProvider.h"
#include "GLFrameBufferObject.h"
#include "Define.h"

namespace lcf {
    class Light : public Object3D
    {
    public:   
        using SharedPtr = std::shared_ptr<Light>;
        Light() = default;
        Object3DType type() const override;
        void draw() override;
        virtual LightType lightType() const = 0;
        virtual void bind() { }
        virtual void release() { }
        virtual void bindAsShadowMap(int texture_unit) { }
        virtual UniformList asUniformList();
        void setColor(const Vector3D& color);
        void setIntensity(float intensity);
        Vector3D direction();
        void setShadowMapUnit(int unit);
    protected:
        std::string uniformName(const std::string &name) const;
    public:
        ProjectionProvider m_projection_provider;
        Vector3D m_direction;
        Vector3D m_color = {1.0f, 1.0f, 1.0f};
        float m_intensity = 1.0f;
        int m_shadow_map_unit = 0;
    };
}