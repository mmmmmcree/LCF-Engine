#include "DirectionalLight.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include "ShaderManager.h"
#include "GLHelper.h"

lcf::DirectionalLight::DirectionalLight() : Light()
{
    m_projection_provider.setType(ProjectionProvider::Type::Orthographic);
    m_projection_provider.setOrthoSize(13.0f);
    m_projection_provider.setNearPlane(1.0f);
    m_projection_provider.setFarPlane(80.0f);
    m_light_index = s_light_count;
    s_light_count++;
    m_fbo = SingleAttachmentFBO::createUnique(4096, 4096, SingleAttachmentFBO::DepthMap);
}

lcf::UniformList lcf::DirectionalLight::asUniformList()
{
    UniformList uniform_list = Light::asUniformList();
    uniform_list.emplace_back(SingleUniform(uniformName("direction"), [this] { return this->direction(); }));
    return uniform_list;
}

lcf::SingleAttachmentFBO::UniquePtr &lcf::DirectionalLight::fbo()
{
    return m_fbo;
}

lcf::DirectionalLight::SharedPtr lcf::DirectionalLight::createShared()
{
    return std::make_shared<DirectionalLight>();
}

lcf::LightType lcf::DirectionalLight::lightType() const
{
    return LightType::Directional;
}

void lcf::DirectionalLight::bind()
{
    const auto &shadow_shader = ShaderManager::instance()->getShadowShader(this->lightType(), false);
    const auto &animated_shadow_shader = ShaderManager::instance()->getShadowShader(this->lightType(), true);
    GLHelper::setShaderUniform(shadow_shader.get(), {"light_index", m_light_index});
    GLHelper::setShaderUniform(animated_shadow_shader.get(), {"light_index", m_light_index});
    constexpr int offset = 64 + 64 + 16;
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    if (not s_ssbo) { gl->glGenBuffers(1, &s_ssbo); }
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, s_ssbo);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, s_ssbo);
    if (s_light_count * offset > s_ssbo_size) {
        s_ssbo_size = s_light_count * offset + 5;
        gl->glBufferData(GL_SHADER_STORAGE_BUFFER, s_ssbo_size, nullptr, GL_DYNAMIC_DRAW);
    }
    if (m_world_need_update) {
        m_light_matrix = m_projection_provider.projectionMatrix() * this->inversedWorldMatrix();
        gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_light_index * offset, 64, m_light_matrix.constData());
        gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_light_index * offset + 64, 64, this->inversedWorldMatrix().constData());
        Vector3D direction = this->direction();
        gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_light_index * offset + 128, 12, &direction);
    }
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    m_fbo->bind();
    gl->glClear(GL_DEPTH_BUFFER_BIT);
}

void lcf::DirectionalLight::release()
{
    m_fbo->release();
}

void lcf::DirectionalLight::bindAsShadowMap(int texture_unit)
{
    m_fbo->texture().bind(texture_unit);
    // auto gl = QOpenGLContext::currentContext()->functions();
    // gl->glActiveTexture(GL_TEXTURE0 + texture_unit);
    // gl->glBindTexture(GL_TEXTURE_2D, m_fbo->texture());
}
