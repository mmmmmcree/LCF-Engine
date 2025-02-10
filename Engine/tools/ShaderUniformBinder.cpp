#include "ShaderUniformBinder.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "GLHelper.h"
#include "TextureDispatcher.h"

lcf::ShaderUniformBinder::ShaderUniformBinder(const ShaderUniformBinder &other) :
    m_bound_count(0),
    m_shader(other.m_shader),
    m_uniforms(other.m_uniforms),
    m_name_to_index_map(other.m_name_to_index_map),
    m_unset_uniforms(other.m_unset_uniforms)
{

}

lcf::ShaderUniformBinder::SharedPtr lcf::ShaderUniformBinder::createShared()
{
    return std::make_shared<ShaderUniformBinder>();
}

lcf::ShaderUniformBinder::SharedPtr lcf::ShaderUniformBinder::createShared(const SharedGLShaderProgramPtr &shader)
{
    SharedPtr result = createShared();
    result->setShader(shader);
    return result;
}

lcf::ShaderUniformBinder::SharedPtr lcf::ShaderUniformBinder::createShared(const ShaderUniformBinder &other)
{
    return std::make_shared<ShaderUniformBinder>(other);
}

void lcf::ShaderUniformBinder::setShader(const SharedGLShaderProgramPtr &shader)
{
    m_shader = shader;
    m_unset_uniforms.insert(m_unset_uniforms.end(), m_uniforms.begin(), m_uniforms.end());
    m_uniforms.clear();
    m_name_to_index_map.clear();
    if (not m_shader) { return; }
    this->getSamplerUniformsFromShader();
    auto unset_uniforms = m_unset_uniforms;
    m_unset_uniforms.clear();
    this->setUniforms(unset_uniforms);
}

void lcf::ShaderUniformBinder::setUniform(const Uniform &uniform)
{
    if (not m_shader) {
        m_unset_uniforms.push_back(uniform);
        return;
    }
    std::visit([&](auto &&arg) {
        const auto &name = arg.name();
        int location = m_shader->uniformLocation(name.c_str());
        if (location == -1) {
            m_unset_uniforms.push_back(uniform);
            return;
        }
        if (m_name_to_index_map.find(name) == m_name_to_index_map.end()) {
            m_name_to_index_map[name] = -1;
        }
        int &index = m_name_to_index_map[name];
        if (index == -1) {
            index = static_cast<int>(m_uniforms.size());
            m_uniforms.push_back(arg);
        } else {
            m_uniforms[index] = arg;
        }
        std::visit([location](auto &&arg) { arg.setLocation(location); }, m_uniforms[index]);
    }, uniform);
}

void lcf::ShaderUniformBinder::setUniforms(const UniformList &uniforms)
{
    for (auto &uniform : uniforms) {
        this->setUniform(uniform);
    }
}

void lcf::ShaderUniformBinder::setSingleUniform(const SingleUniform &uniform)
{
    this->setUniform(uniform);
}

void lcf::ShaderUniformBinder::setSingleUniforms(const SingleUniformList & uniforms)
{
    for (const auto &uniform : uniforms) {
        this->setSingleUniform(uniform);
    }
}

void lcf::ShaderUniformBinder::setArrayUniform(const ArrayUniform &uniform)
{
    this->setUniform(uniform);
}

void lcf::ShaderUniformBinder::setArrayUniforms(const ArrayUniformList & uniforms)
{
    for (const auto &uniform : uniforms) {
        this->setArrayUniform(uniform);
    }
}

const lcf::SharedGLShaderProgramPtr &lcf::ShaderUniformBinder::shader() const
{
    return m_shader;
}

void lcf::ShaderUniformBinder::bind()
{
    if (not m_shader) { return; }
    int texture_unit = 0;
    for (const auto &name : m_sampler_names) {
        auto texture = TextureDispatcher::instance()->getTextureByName(name);
        if (not texture.isValid()) { continue; }
        m_shader->setUniformValue(name.c_str(), texture_unit);
        texture.bind(texture_unit++);
        m_binding_textures.push_back(texture);
    }
    if (++m_bound_count > 1) { return; }
    m_shader->bind();
    for (auto &uniform : m_uniforms) {
        std::visit([&](auto &&arg) {
            arg.bind(m_shader.get());
        }, uniform);
    }
}

void lcf::ShaderUniformBinder::release()
{
    if (not m_shader) { return; }
    int texture_unit = 0;
    for (auto &texture : m_binding_textures) {
        texture.release(texture_unit++);
    }
    m_binding_textures.clear();
    if (--m_bound_count > 0) { return; }
    m_shader->release();
}

bool lcf::ShaderUniformBinder::hasShader() const
{
    return m_shader.get();
}

void lcf::ShaderUniformBinder::getSamplerUniformsFromShader()
{
    if (not m_shader) { return; }
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    int uniform_num = 0;
    gl->glGetProgramiv(m_shader->programId(), GL_ACTIVE_UNIFORMS, &uniform_num);
    m_sampler_names.clear();
    for (int i = 0; i < uniform_num; ++i) {
        GLchar name[256];
        GLsizei name_length;
        GLint size;
        GLenum type;
        gl->glGetActiveUniform(m_shader->programId(), i, sizeof(name), &name_length, &size, &type, name);
        if (not GLHelper::isSampler(type)) { continue; }
        m_sampler_names.push_back(name);
    }
}
