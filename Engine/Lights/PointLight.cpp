#include "PointLight.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "Geometry.h"
#include "ShaderManager.h"
#include "GLHelper.h"

lcf::PointLight::PointLight()
{
    m_projection_provider.setType(ProjectionProvider::Type::Perspective);
    m_projection_provider.setNearPlane(1.0f);
    m_projection_provider.setFarPlane(100.0f);
    m_projection_provider.setFov(90.0f);
    m_light_index = s_light_count++;
    m_fbo = DepthCubeMapFBO::createUnique(4096);
    m_projection_provider.setAspect(m_fbo->width(), m_fbo->height());
}

lcf::PointLight::SharedPtr lcf::PointLight::createShared()
{
    return std::make_shared<PointLight>();
}

void lcf::PointLight::draw()
{
    static auto shader = ShaderManager::instance()->get(ShaderManager::SingleColor);
    shader->bind();
    shader->setUniformValue("model", this->worldMatrix());
    shader->setUniformValue("color", m_color);
    Geometry::sphere()->draw();
    shader->release();
}

lcf::LightType lcf::PointLight::lightType() const
{
    return LightType::Point;
}

lcf::UniformList lcf::PointLight::asUniformList()
{
    UniformList uniform_list = Light::asUniformList();
    uniform_list.emplace_back(SingleUniform(uniformName("position"), [this] { return this->worldPosition(); }));
    uniform_list.emplace_back(SingleUniform(uniformName("constant"), [this] { return this->m_constant; }));
    uniform_list.emplace_back(SingleUniform(uniformName("linear"), [this] { return this->m_linear; }));
    uniform_list.emplace_back(SingleUniform(uniformName("quadratic"), [this] { return this->m_quadratic; }));
    uniform_list.emplace_back(SingleUniform(uniformName("index"), [this] { return m_light_index; }));
    uniform_list.emplace_back(SingleUniform(uniformName("far_plane"), [this] { return m_projection_provider.farPlane(); }));
    uniform_list.emplace_back(SingleUniform(uniformName("shadow_map"), [this] { return m_shadow_map_unit; }));
    return uniform_list;
}

void lcf::PointLight::bind()
{
    m_fbo->depthAttachment().release(m_shadow_map_unit);
    const auto &shadow_shader = ShaderManager::instance()->getShadowShader(this->lightType(), false);
    const auto &animated_shadow_shader = ShaderManager::instance()->getShadowShader(this->lightType(), true);
    GLHelper::setShaderUniform(shadow_shader.get(), {"light_index", m_light_index});
    GLHelper::setShaderUniform(animated_shadow_shader.get(), {"light_index", m_light_index});
    constexpr int offset = 64 * 6 + 16;
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    if (not s_ssbo) { gl->glGenBuffers(1, &s_ssbo); }
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, s_ssbo);
    gl->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, s_ssbo);
    if (s_light_count * offset > s_ssbo_size) {
        s_ssbo_size = s_light_count * offset + 5;
        gl->glBufferData(GL_SHADER_STORAGE_BUFFER, s_ssbo_size, nullptr, GL_DYNAMIC_DRAW);
    }
    if (m_world_need_update) {
        this->updateLightMatrices();
        for (int i = 0; i < m_light_matrices.size(); ++i) {
            gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_light_index * offset + i * 64, 64, m_light_matrices[i].constData());
        }
        Vector3D light_pos = this->worldPosition();
        gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_light_index * offset + 64 * 6, 12, &light_pos);
        gl->glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_light_index * offset + 64 * 6 + 12, 4, &m_projection_provider.farPlane());
    }
    gl->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    m_fbo->bind();
}

void lcf::PointLight::release()
{
    m_fbo->release();
    m_fbo->depthAttachment().bind(m_shadow_map_unit);
}

int lcf::PointLight::index() const
{
    return m_light_index;
}

const lcf::NativeTextureWrapper &lcf::PointLight::shadowMapTexture() const
{
    return m_fbo->depthAttachment();
}

void lcf::PointLight::updateLightMatrices()
{
    Vector3D positive_x = Vector3D(1, 0, 0);
    Vector3D positive_y = Vector3D(0, 1, 0);
    Vector3D positive_z = Vector3D(0, 0, 1);
    const auto &projection = m_projection_provider.projectionMatrix();
    const auto &light_pos = this->worldPosition();
    Matrix4x4 view; view.lookAt(light_pos, light_pos + positive_x, -positive_y);
    m_light_matrices[0] = projection * view;
    view.setToIdentity(); view.lookAt(light_pos, light_pos - positive_x, -positive_y);
    m_light_matrices[1] = projection * view;
    view.setToIdentity(); view.lookAt(light_pos, light_pos + positive_y, positive_z);
    m_light_matrices[2] = projection * view;
    view.setToIdentity(); view.lookAt(light_pos, light_pos - positive_y, -positive_z);
    m_light_matrices[3] = projection * view;
    view.setToIdentity(); view.lookAt(light_pos, light_pos + positive_z, -positive_y);
    m_light_matrices[4] = projection * view;
    view.setToIdentity(); view.lookAt(light_pos, light_pos - positive_z, -positive_y);
    m_light_matrices[5] = projection * view;
}