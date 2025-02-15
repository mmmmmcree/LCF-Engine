// #include "ShaderUniformBinder.h"
// #include <QOpenGLContext>
// #include <QOpenGLFunctions>
// #include <QOpenGLExtraFunctions>
// #include "GLHelper.h"
// #include "TextureDispatcher.h"

// lcf::ShaderUniformBinder::ShaderUniformBinder(const ShaderUniformBinder &other) :
//     m_shader(other.m_shader)
// {

// }

// lcf::ShaderUniformBinder::SharedPtr lcf::ShaderUniformBinder::createShared()
// {
//     return std::make_shared<ShaderUniformBinder>();
// }

// lcf::ShaderUniformBinder::SharedPtr lcf::ShaderUniformBinder::createShared(const GLShaderProgram::SharedPtr &shader)
// {
//     SharedPtr result = createShared();
//     result->setShader(shader);
//     return result;
// }

// lcf::ShaderUniformBinder::SharedPtr lcf::ShaderUniformBinder::createShared(const ShaderUniformBinder &other)
// {
//     return std::make_shared<ShaderUniformBinder>(other);
// }

// void lcf::ShaderUniformBinder::setShader(const GLShaderProgram::SharedPtr &shader)
// {
//     m_shader = shader;
// }

// const lcf::GLShaderProgram::SharedPtr &lcf::ShaderUniformBinder::shader() const
// {
//     return m_shader;
// }

// void lcf::ShaderUniformBinder::bind()
// {
//     if (not m_shader) { return; }
//     m_shader->bind();
// }

// void lcf::ShaderUniformBinder::release()
// {
//     if (not m_shader) { return; }
//     m_shader->release();
// }

// bool lcf::ShaderUniformBinder::hasShader() const
// {
//     return m_shader.get();
// }