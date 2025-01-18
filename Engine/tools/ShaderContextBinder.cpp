#include "ShaderContextBinder.h"
#include "ControlManager.h"

lcf::ShaderContextBinder::ShaderContextBinder()
{
    m_material_controller = MaterialController::createShared();
    m_shader_uniform_binder = ShaderUniformBinder::createShared();
}

lcf::ShaderContextBinder::ShaderContextBinder(const ShaderContextBinder &other)
{
    m_material_controller = other.m_material_controller;
    m_shader_uniform_binder = other.m_shader_uniform_binder;
}

void lcf::ShaderContextBinder::bind()
{
    m_shader_uniform_binder->bind();
    m_material_controller->bind();
}

void lcf::ShaderContextBinder::release()
{
    m_material_controller->release();
    m_shader_uniform_binder->release();
}

void lcf::ShaderContextBinder::setMaterialController(MaterialController::SharedPtr material_controller)
{
    m_material_controller = material_controller;
    m_shader_uniform_binder->setUniforms(m_material_controller->asUniformList());
}

const lcf::MaterialController::SharedPtr &lcf::ShaderContextBinder::materialController() const
{
    return m_material_controller;
}

void lcf::ShaderContextBinder::setMaterialType(MaterialType type)
{
    m_material_controller->setMaterialType(type);
    m_shader_uniform_binder->setUniforms(m_material_controller->asUniformList());
}

lcf::MaterialType lcf::ShaderContextBinder::materialType() const
{
    return m_material_controller->materialType();
}

void lcf::ShaderContextBinder::setShader(const SharedGLShaderProgramPtr &shader)
{
    m_shader_uniform_binder->setShader(shader);
    m_shader_uniform_binder->setUniforms(m_material_controller->asUniformList());
    auto current_scene = ControlManager::instance()->currentScene();
    m_shader_uniform_binder->setUniforms(current_scene->lights().asUniformList());
}

void lcf::ShaderContextBinder::setShaderUniformBinder(const ShaderUniformBinder::SharedPtr & shader_uniform_binder)
{
    m_shader_uniform_binder = shader_uniform_binder;
}

const lcf::ShaderUniformBinder::SharedPtr & lcf::ShaderContextBinder::shaderUniformBinder() const
{
    return m_shader_uniform_binder;
}

void lcf::ShaderContextBinder::setUniform(const Uniform &uniform)
{
    m_shader_uniform_binder->setUniform(uniform);
}

void lcf::ShaderContextBinder::setUniforms(const UniformList &uniforms)
{
    m_shader_uniform_binder->setUniforms(uniforms);
}

void lcf::ShaderContextBinder::setSingleUniform(const SingleUniform &uniform)
{
    m_shader_uniform_binder->setSingleUniform(uniform);
}

void lcf::ShaderContextBinder::setSingleUniforms(const SingleUniformList &uniforms)
{
    m_shader_uniform_binder->setSingleUniforms(uniforms);
}

void lcf::ShaderContextBinder::setArrayUniform(const ArrayUniform &uniform)
{
    m_shader_uniform_binder->setArrayUniform(uniform);
}

void lcf::ShaderContextBinder::setArrayUniforms(const ArrayUniformList &uniforms)
{
    m_shader_uniform_binder->setArrayUniforms(uniforms);
}

const lcf::SharedGLShaderProgramPtr &lcf::ShaderContextBinder::shader() const
{
    return m_shader_uniform_binder->shader();
}

bool lcf::ShaderContextBinder::isValid() const
{
    return m_shader_uniform_binder->hasShader();
}
