#include "ShaderUniformBinder.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

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
    m_name_to_index_map.clear();
    m_unset_uniforms.insert(m_unset_uniforms.end(), m_uniforms.begin(), m_uniforms.end());
    m_uniforms.clear();
    if (not m_shader) { return; }
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
        auto &name = arg.name();
        int location = m_shader->uniformLocation(name.c_str());
        if (location == -1) {
            m_unset_uniforms.push_back(uniform);
            return;
        }
        auto iter = m_name_to_index_map.find(name);
        if (iter != m_name_to_index_map.end()) {
            m_uniforms[iter->second] = arg;
            this->setUniformLocation(m_uniforms[iter->second], location);
            return;
        }
        m_name_to_index_map[name] = static_cast<int>(m_uniforms.size());
        m_uniforms.push_back(arg);
        this->setUniformLocation(m_uniforms.back(), location);
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
    ++m_bound_count;
    if (m_bound_count > 1) { return; }
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
    --m_bound_count;
    if (m_bound_count == 0) {
        m_shader->release();
    }
}

bool lcf::ShaderUniformBinder::hasShader() const
{
    return m_shader.get();
}

void lcf::ShaderUniformBinder::setUniformLocation(Uniform &uniform, int location)
{
    std::visit([&](auto &&arg) {
        arg.setLocation(location);
    }, uniform);
}
