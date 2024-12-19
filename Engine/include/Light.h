#pragma once

#include "Object3D.h"
#include "Uniform.h"
#include "ProjectionProvider.h"
#include "GLFrameBufferObject.h"

namespace lcf {
    class Light : public Object3D
    {
    public:   
        using SharedPtr = std::shared_ptr<Light>;
        Light();
        void draw() override;
        virtual void bind(int light_index = 0) { }
        virtual void release() { }
        virtual void bindAsShadowMap(int texture_unit) { }
        virtual UniformList asUniformList();
        void setColor(const Vector3D& color);
        void setDiffuseIntensity(float intensity);
        void setSpecularIntensity(float intensity);
        void setAmbientIntensity(float intensity);
        Vector3D direction();
    protected:
        std::string uniformName(const std::string &name) const;
    public:
        ProjectionProvider m_projection_provider;
        Vector3D m_color = {1.0f, 1.0f, 1.0f};
        float m_diffuse_intensity = 1.0f;
        float m_specular_intensity = 0.5f;
        float m_ambient_intensity = 0.2f;
    };
}