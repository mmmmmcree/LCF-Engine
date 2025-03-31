#include "PointLight.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "Geometry.h"
#include "ShaderManager.h"
#include "GLHelper.h"
#include "TextureDispatcher.h"
#include "GLFunctions.h"

lcf::PointLight::PointLight()
{
    m_projection_provider.setType(ProjectionProvider::Type::Perspective);
    m_projection_provider.setNearPlane(1.0f);
    m_projection_provider.setFarPlane(100.0f);
    m_projection_provider.setFov(90.0f);
    m_fbo = DepthCubeMapFBO::createUnique(4096);
    m_projection_provider.setAspect(m_fbo->width(), m_fbo->height());
    m_constant.setValue(1.0f);
    m_linear.setValue(0.09f);
    m_far_plane.setValue(100.0f);
    m_quadratic.setValue(0.032f);
    m_light_index = s_light_count++;

    //- 设置ssbo
    if (not s_ssbo) {
        s_ssbo.setBindingPoint(3);
        s_ssbo.setDataSizes({64, 64, 64, 64, 64, 64, 12, 4});
        s_ssbo.create();
    }
    s_ssbo.fitToSize(s_light_count);
}

lcf::PointLight::SharedPtr lcf::PointLight::createShared()
{
    return std::make_shared<PointLight>();
}

void lcf::PointLight::draw()
{
    static auto shader = ShaderManager::instance()->get(ShaderManager::SingleColor);
    shader->bindWithTextures();
    shader->setUniformValue("model", this->getWorldMatrix());
    shader->setUniformValue("color", m_color.value());
    Geometry::sphere()->draw();
    shader->release();
}

lcf::LightType lcf::PointLight::lightType() const
{
    return LightType::Point;
}

void lcf::PointLight::update()
{
    if (m_transformer.isUpdated()) { return; }
    this->updateLightMatrices();
    s_ssbo.bind();
    for (int i = 0; i < m_light_matrices.size(); ++i) {
        s_ssbo.updateData(this->index(), i, m_light_matrices[i].constData());
    }
    Vector3D light_pos = this->getWorldPosition();
    s_ssbo.updateData(this->index(), 6, &light_pos);
    s_ssbo.updateData(this->index(), 7, &m_projection_provider.farPlane());
    s_ssbo.release();
    Light::update();
}

void lcf::PointLight::bind()
{
    const auto &shadow_shader = ShaderManager::instance()->getShadowShader(this->lightType(), false);
    const auto &animated_shadow_shader = ShaderManager::instance()->getShadowShader(this->lightType(), true);
    GLHelper::setShaderUniform(shadow_shader.get(), {"light_index", m_light_index});
    GLHelper::setShaderUniform(animated_shadow_shader.get(), {"light_index", m_light_index});
    m_fbo->bind();
}

void lcf::PointLight::release()
{
    m_fbo->release();
    TextureDispatcher::instance()->setTextureByName(uniformName("shadow_map"), m_fbo->depthAttachment());
}

int lcf::PointLight::index() const
{
    return m_light_index;
}

void lcf::PointLight::setName(std::string_view name)
{
    Light::setName(name);
    m_constant.setName(this->uniformName("constant"));
    m_linear.setName(this->uniformName("linear"));
    m_quadratic.setName(this->uniformName("quadratic"));
    m_far_plane.setName(this->uniformName("far_plane"));
    m_color.setName(this->uniformName("color"));
}

void lcf::PointLight::updateLightMatrices()
{
    Vector3D positive_x = Vector3D(1, 0, 0);
    Vector3D positive_y = Vector3D(0, 1, 0);
    Vector3D positive_z = Vector3D(0, 0, 1);
    const auto &projection = m_projection_provider.projectionMatrix();
    const auto &light_pos = this->getWorldPosition();
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