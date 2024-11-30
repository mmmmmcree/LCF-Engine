#include "ShaderManager.h"
#include <QFileInfo>

lcf::ShaderManager *lcf::ShaderManager::instance()
{
    static ShaderManager s_instance;
    return &s_instance;
}

void lcf::ShaderManager::initialize()
{
    this->instance();
}

std::unique_ptr<lcf::GLShaderProgram> lcf::ShaderManager::load(const ShaderInfos &shader_infos)
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

lcf::GLShaderProgram *lcf::ShaderManager::load(const QString &name, const ShaderInfos &shader_infos)
{
    if (m_shaders.find(name) != m_shaders.end()) {
        qDebug() << "Shader already loaded: " << name;
    } else {
        m_shaders.emplace(name, load(shader_infos));
    }
    return m_shaders[name].get();
}

lcf::GLShaderProgram *lcf::ShaderManager::get(const QString &name)
{
    auto iter = m_shaders.find(name);
    if (iter != m_shaders.end()) { return iter->second.get(); }
    return nullptr;
}

const QString &lcf::ShaderManager::defaultPath()
{
    static QString s_default_path = QString(SOURCE_DIR) + QString("/shaders/");
    return s_default_path;
}

lcf::ShaderManager::ShaderManager() : QObject()
{
    GLShaderProgram *shader = nullptr;
    shader = load("texture_debug_2D", {
        {QOpenGLShader::Vertex, defaultPath() + "simple2D.vert"}, 
        {QOpenGLShader::Fragment, defaultPath() + "texture_debug.frag"}, 
    });
    shader->setUniformValue("channel0", 0);
    shader = load("texture_debug_3D", {
        {QOpenGLShader::Vertex, defaultPath() + "simple3D.vert"}, 
        {QOpenGLShader::Fragment, defaultPath() + "texture_debug.frag"}, 
    });
    shader->setUniformValue("tex", 0);
    shader = load("geometry_debug_3D", {
        {QOpenGLShader::Vertex, defaultPath() + "simple3D.vert"}, 
        {QOpenGLShader::Fragment, defaultPath() + "geometry_debug.frag"}, 
    });
    shader = load("animation_debug_3D", {
        {QOpenGLShader::Vertex, defaultPath() + "animation3D.vert"}, 
        {QOpenGLShader::Fragment, defaultPath() + "texture_debug.frag"}, 
    });
}
