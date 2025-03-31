#include "DirectionalLight.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include "ShaderManager.h"
#include "GLHelper.h"
#include "TextureDispatcher.h"
#include "GLFunctions.h"

lcf::DirectionalLight::DirectionalLight() : Light()
{
    m_projection_provider.setType(ProjectionProvider::Type::Orthographic);
    m_projection_provider.setOrthoSize(13.0f);
    m_projection_provider.setNearPlane(1.0f);
    m_projection_provider.setFarPlane(80.0f);
    m_near_plane.setValue(m_projection_provider.nearPlane());
    m_frustum_size.setValue(m_projection_provider.nearPlaneWidth());
    m_light_index = s_light_count;
    s_light_count++;
    m_fbo = DepthMapFBO::createUnique(4096, 4096);
    
    //- 设置ssbo
    if (not s_ssbo) {
        s_ssbo.setBindingPoint(2);
        s_ssbo.setDataSizes({64, 64, 16});
        s_ssbo.create();
    }
    s_ssbo.fitToSize(s_light_count);
}

void lcf::DirectionalLight::setName(std::string_view name)
{
    Light::setName(name);
    m_light_size.setName(uniformName("light_size"));
    m_near_plane.setName(uniformName("near_plane"));
    m_frustum_size.setName(uniformName("frustum_size"));
}

lcf::DirectionalLight::SharedPtr lcf::DirectionalLight::createShared()
{
    return std::make_shared<DirectionalLight>();
}

lcf::LightType lcf::DirectionalLight::lightType() const
{
    return LightType::Directional;
}

void lcf::DirectionalLight::update()
{
    if (m_transformer.isUpdated()) { return; }
    m_light_matrix = m_projection_provider.projectionMatrix() * this->getInversedWorldMatrix();
    Vector3D direction = this->orientation();
    s_ssbo.bind();
    s_ssbo.updateData(m_light_index, 0, m_light_matrix.constData());
    s_ssbo.updateData(m_light_index, 1, this->getInversedWorldMatrix().constData());
    s_ssbo.updateData(m_light_index, 2, &direction);
    s_ssbo.release();
    m_light_size.setValue(max(this->scale()));
    Light::update();
}

int lcf::DirectionalLight::index() const
{
    return m_light_index;
}

void lcf::DirectionalLight::bind()
{
    const auto &shadow_shader = ShaderManager::instance()->getShadowShader(this->lightType(), false);
    const auto &animated_shadow_shader = ShaderManager::instance()->getShadowShader(this->lightType(), true);
    GLHelper::setShaderUniform(shadow_shader.get(), {"light_index", m_light_index});
    GLHelper::setShaderUniform(animated_shadow_shader.get(), {"light_index", m_light_index});
    m_fbo->bind();
}

void lcf::DirectionalLight::release()
{
    m_fbo->release();
    TextureDispatcher::instance()->setTextureByName(uniformName("shadow_map"), m_fbo->depthAttachment());
}
