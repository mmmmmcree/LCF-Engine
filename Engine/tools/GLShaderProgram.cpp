#include "GLShaderProgram.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "Matrix.h"
#include "UniformManager.h"
#include "GLHelper.h"
#include "TextureDispatcher.h"
#include "Uniform.h"
#include "GLFunctions.h"

lcf::GLShaderProgram::GLShaderProgram(QObject *parent) : QOpenGLShaderProgram(parent)
{
}

bool lcf::GLShaderProgram::link()
{
    bool success = QOpenGLShaderProgram::link();
    if (not success) {
        qDebug() << "lcf::GLShaderProgram::link() failed: " << this->log();
        return false;
    }
    auto gl = QOpenGLContext::currentContext()->functions();
    int uniform_num = 0;
    gl->glGetProgramiv(this->programId(), GL_ACTIVE_UNIFORMS, &uniform_num);
    for (int i = 0; i < uniform_num; ++i) {
        GLchar name[256];
        GLsizei name_length;
        GLint size;
        GLenum type;
        gl->glGetActiveUniform(this->programId(), i, sizeof(name), &name_length, &size, &type, name);
        this->assignDefaultValueToUniform(name, type);
        m_uniform_names.insert(name);
        if (not GLHelper::isSampler(type)) { continue; }
        m_sampler_names.push_back(name);
    }
    for (const auto &exist_uniform : UniformManager::instance()->existedUniforms()) {
        if (not m_uniform_names.contains(exist_uniform->name())) { continue; }
        m_unset_uniforms.push_back(exist_uniform);
    }
    connect(UniformManager::instance(), &UniformManager::uniformUpdated, this, &GLShaderProgram::onUniformUpdated);
    return success;
}

bool lcf::GLShaderProgram::bindWithTextures()
{
    bool success = this->bind();
    if (not success) { return false; }
    int texture_unit = 0;
    for (const auto &name : m_sampler_names) {
        auto texture = TextureDispatcher::instance()->getTextureByName(name);
        if (not texture.isValid()) { continue; }
        this->setUniformValue(name.c_str(), texture_unit);
        texture.bind(texture_unit++);
        m_binding_textures.push_back(texture);
    }
    return success;
}

bool lcf::GLShaderProgram::bind()
{
    bool success = QOpenGLShaderProgram::bind();
    if (not success) { return false; }
    if (m_unset_uniforms.empty()) { return success; }
    for (auto uniform : m_unset_uniforms) {
        if (not UniformManager::instance()->existUniform(uniform)) { continue; }
        uniform->bind(this);
    }
    m_unset_uniforms.clear();
    return success;
}

void lcf::GLShaderProgram::release()
{
    QOpenGLShaderProgram::release();
    if (m_binding_textures.empty()) { return; }
    int texture_unit = 0;
    for (auto &texture : m_binding_textures) {
        texture.release(texture_unit++);
    }
    m_binding_textures.clear();
}

void lcf::GLShaderProgram::setUniformValue(const char *name, const GLMVector3D &value)
{
    this->setUniformValue(this->uniformLocation(name), value);
}

void lcf::GLShaderProgram::setUniformValue(int location, const GLMVector3D &value)
{
    if (location < 0) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glUniform3fv(location, 1, reinterpret_cast<const GLfloat*>(&value));
}

void lcf::GLShaderProgram::setUniformValueArray(const char *name, const GLMVector3D *values, int count)
{
    this->setUniformValueArray(this->uniformLocation(name), values, count);
}

void lcf::GLShaderProgram::setUniformValueArray(int location, const GLMVector3D *values, int count)
{
    if (location < 0) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glUniform3fv(location, count, reinterpret_cast<const GLfloat*>(values));
}

void lcf::GLShaderProgram::setUniformValue(const char *name, const GLMVector4D &value)
{
    this->setUniformValue(this->uniformLocation(name), value);
}

void lcf::GLShaderProgram::setUniformValue(int location, const GLMVector4D &value)
{
    if (location < 0) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glUniform4fv(location, 1, reinterpret_cast<const GLfloat*>(&value));
}

void lcf::GLShaderProgram::setUniformValueArray(const char *name, const GLMVector4D *values, int count)
{
    this->setUniformValueArray(this->uniformLocation(name), values, count);
}

void lcf::GLShaderProgram::setUniformValueArray(int location, const GLMVector4D *values, int count)
{
    if (location < 0) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glUniform4fv(location, count, reinterpret_cast<const GLfloat*>(values));
}

void lcf::GLShaderProgram::setUniformValue(const char *name, const GLMMatrix3x3 &value)
{
    this->setUniformValue(this->uniformLocation(name), value);
}

void lcf::GLShaderProgram::setUniformValue(int location, const GLMMatrix3x3 &value)
{
    if (location < 0) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glUniformMatrix3fv(location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
}

void lcf::GLShaderProgram::setUniformValueArray(const char *name, const GLMMatrix3x3 *values, int count)
{
    this->setUniformValueArray(this->uniformLocation(name), values, count);
}

void lcf::GLShaderProgram::setUniformValueArray(int location, const GLMMatrix3x3 *values, int count)
{
    if (location < 0) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glUniformMatrix3fv(location, count, GL_FALSE, reinterpret_cast<const GLfloat*>(values));
}

void lcf::GLShaderProgram::setUniformValue(const char *name, const GLMMatrix4x4 &value)
{
    this->setUniformValue(this->uniformLocation(name), value);
}

void lcf::GLShaderProgram::setUniformValue(int location, const GLMMatrix4x4 & value)
{
    if (location < 0) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glUniformMatrix4fv(location, 1, GL_FALSE, value.constData());
}

void lcf::GLShaderProgram::setUniformValueArray(const char *name, const GLMMatrix4x4 *values, int count)
{
    this->setUniformValueArray(this->uniformLocation(name), values, count);
}

void lcf::GLShaderProgram::setUniformValueArray(int location, const GLMMatrix4x4 *values, int count)
{
    if (location < 0) { return; }
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->glUniformMatrix4fv(location, count, GL_FALSE, reinterpret_cast<const GLfloat*>(values));
}

void lcf::GLShaderProgram::assignDefaultValueToUniform(const char *uniform_name, unsigned int uniform_type)
{
    switch (uniform_type) {
        case GL_FLOAT: { this->setUniformValue(uniform_name, 0.0f);  } break;
        // case GL_FLOAT_VEC2: { this->setUniformValue(uniform_name, Vector2D(0.0f, 0.0f)); } break;
        case GL_FLOAT_VEC3: { this->setUniformValue(uniform_name, Vector3D(0.0f, 0.0f, 0.0f)); } break;
        case GL_FLOAT_VEC4: { this->setUniformValue(uniform_name, Vector4D(0.0f, 0.0f, 0.0f, 0.0f)); } break;
        case GL_FLOAT_MAT3: { this->setUniformValue(uniform_name, Matrix3x3()); } break;
        case GL_FLOAT_MAT4: { this->setUniformValue(uniform_name, Matrix4x4()); } break;
        default: { this->setUniformValue(uniform_name, 0); } break;
    }
}

void lcf::GLShaderProgram::onUniformUpdated(const Uniform *uniform)
{
    if (m_uniform_names.contains(uniform->name())) {
        m_unset_uniforms.push_back(uniform);
    }
}