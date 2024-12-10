#include "ShaderUniformBinder.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>


lcf::ShaderUniformBinder::ShaderUniformBinder(const SharedGLShaderProgramPtr &shader) :
    m_shader(shader),
    m_bound_count(0)
{
}

void lcf::ShaderUniformBinder::setUniform(const Uniform &uniform)
{
    std::visit([&](auto &&arg) {
        auto &name = arg.name();
        int location = m_shader->uniformLocation(name.c_str());
        if (location == -1) { return; }
        auto iter = m_name_to_index_map.find(name);
        if (iter != m_name_to_index_map.end()) {
            m_uniforms[iter->second] = arg;
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
    --m_bound_count;
    if (m_bound_count == 0) {
        m_shader->release();
    }
}

void lcf::ShaderUniformBinder::setUniformLocation(Uniform &uniform, int location)
{
    std::visit([&](auto &&arg) {
        arg.setLocation(location);
    }, uniform);
}
