#include "DirectionalLight.h"
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>

lcf::DirectionalLight::DirectionalLight() : Light()
{
    m_projection_provider.setType(ProjectionProvider::Type::Orthographic);
    m_projection_provider.setOrthoSize(13.0f);
    m_projection_provider.setNearPlane(1.0f);
    m_projection_provider.setFarPlane(80.0f);
    // m_projection_provider.setType(ProjectionProvider::Type::Perspective);
    s_light_count++;
    m_fbo = std::make_unique<FrameBufferObject>(4096, 4096);
    m_fbo->addDepthTexture();
}

lcf::UniformList lcf::DirectionalLight::asUniformList()
{
    UniformList uniform_list = Light::asUniformList();
    uniform_list.emplace_back(SingleUniform(uniformName("direction"), [this] { return this->direction(); }));
    return uniform_list;
}

lcf::FrameBufferObject::UniquePtr &lcf::DirectionalLight::fbo()
{
    return m_fbo;
}

void lcf::DirectionalLight::bind(int light_index)
{
    m_light_matrix = m_projection_provider.projectionMatrix() * this->inversedWorldMatrix();
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    if (not s_ssbo) { gl->glGenBuffers(1, &s_ssbo); }
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, s_ssbo);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, s_ssbo);
    if (s_light_count * 64 > s_ssbo_size) {
        s_ssbo_size = 2 * s_light_count * 64;
        gl->glBufferData(GL_SHADER_STORAGE_BUFFER, s_ssbo_size, nullptr, GL_DYNAMIC_DRAW);
    }
    gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, light_index * 64, 64, m_light_matrix.constData());
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
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0 + texture_unit);
    gl->glBindTexture(GL_TEXTURE_2D, m_fbo->takeDepthTexture());
}
