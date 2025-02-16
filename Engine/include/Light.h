#pragma once

#include "Object3D.h"
#include "Uniform.h"
#include "ProjectionProvider.h"
#include "GLFrameBufferObject.h"
#include "Define.h"
#include "SingleUniform.h"

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
        void setColor(const Vector3D& color);
        void setIntensity(float intensity);
        Vector3D direction();
        void setName(std::string_view name) override;
    protected:
        void updateWorldMatrix() override;
        std::string uniformName(const std::string &name) const;
    protected:
        ProjectionProvider m_projection_provider;
        SingleUniform<Vector3D> m_color;
        SingleUniform<float> m_intensity = 1.0f;
        SingleUniform<Vector3D> m_position = Vector3D(0.0f, 0.0f, 0.0f);
        SingleUniform<Vector3D> m_direction = Vector3D(0.0f, 0.0f, 1.0f);
    };
}