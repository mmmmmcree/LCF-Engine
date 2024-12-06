#include "ShaderManager.h"
#include <QFileInfo>
#include "Constants.h"

lcf::ShaderManager *lcf::ShaderManager::instance()
{
    static ShaderManager s_instance;
    return &s_instance;
}

void lcf::ShaderManager::initialize()
{
    this->instance();
}

lcf::UniqueGLShaderProgramPtr lcf::ShaderManager::load(const ShaderInfos &shader_infos)
{
    GLShaderProgram *shader = new GLShaderProgram;
    for (const auto &[type, path] : shader_infos) {
        if (QFileInfo(path).exists()) {
            shader->addShaderFromSourceFile(type, path);
        } else {
            qDebug() << "Shader file not found: " << path;
        }
    }
    if (not shader->link()) {
        qDebug() << shader->log();
        delete shader;
        shader = nullptr;
    }
    return std::unique_ptr<GLShaderProgram>(shader);
}

lcf::SharedGLShaderProgramPtr lcf::ShaderManager::load(const QString &name, const ShaderInfos &shader_infos)
{
    if (m_shaders.find(name) != m_shaders.end()) {
        qDebug() << "Shader already loaded: " << name;
    } else {
        m_shaders.emplace(name, load(shader_infos));
    }
    return m_shaders[name];
}

lcf::SharedGLShaderProgramPtr lcf::ShaderManager::get(const QString &name)
{
    auto iter = m_shaders.find(name);
    if (iter != m_shaders.end()) { return iter->second; }
    return nullptr;
}

lcf::ShaderManager::ShaderManager() : QObject()
{
    SharedGLShaderProgramPtr shader = nullptr;
    shader = load("sampler2D_debug_2D", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "simple2D.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
    shader = load("sampler2D_debug_3D", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
    shader = load("geometry_debug_3D", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "geometry_debug.frag"}, 
    });
    shader = load("animation_debug_3D", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "animation3D.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
    shader = load("skybox", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "skybox.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "samplerSpherical_debug.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
    shader = load("instance_debug_3D", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "instance3D.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
}
