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

lcf::SharedGLShaderProgramPtr lcf::ShaderManager::get(ConfiguredShader type)
{
    SharedGLShaderProgramPtr shader = nullptr;
    switch (type) {
        case ConfiguredShader::Simple2D : { shader = this->get("simple2D"); } break;
        case ConfiguredShader::Simple3D : { shader = this->get("simple3D"); } break;
        case ConfiguredShader::GeometryDebug : { shader = this->get("geometry_debug"); } break;
        case ConfiguredShader::Skybox : { shader = this->get("skybox"); } break;
    }
    return shader;
}

lcf::ShaderManager::ShaderManager() : QObject()
{
    SharedGLShaderProgramPtr shader = nullptr;
    shader = load("simple2D", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "simple2D.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
    shader = load("simple3D", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
    shader = load("geometry_debug", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "geometry_debug.frag"}, 
    });
    shader = load("skybox", {
        {QOpenGLShader::Vertex, lcf::path::shaders_prefix + "skybox.vert"}, 
        {QOpenGLShader::Fragment, lcf::path::shaders_prefix + "sampler_spherical.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
}
