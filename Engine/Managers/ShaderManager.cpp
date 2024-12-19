#include "ShaderManager.h"
#include <QFileInfo>
#include "Constants.h"
#include <sstream>
#include <QRegularExpression>
#include "GLHelper.h"

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
        shader->addShaderFromSourceCode(type, readShaderSourceCode(path));
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
        case ConfiguredShader::ShadowMap : { shader = this->get("shadow_map"); } break;
        case ConfiguredShader::AnimatedShadowMap : { shader = this->get("animated_shadow_map"); } break;
        case ConfiguredShader::DepthDebug : { shader = this->get("depth_debug"); } break;
    }
    return shader;
}

lcf::ShaderManager::ShaderManager() : QObject()
{
    SharedGLShaderProgramPtr shader = nullptr;
    shader = load("simple2D", {
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple2D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    shader = load("simple3D", {
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    shader = load("geometry_debug", {
        {GLShader::Vertex, lcf::path::shaders_prefix + "geometry_debug.vert"}, 
        {GLShader::Geometry, lcf::path::shaders_prefix + "geometry_debug.geom"},
        {GLShader::Fragment, lcf::path::shaders_prefix + "geometry_debug.frag"}, 
    });
    shader = load("skybox", {
        {GLShader::Vertex, lcf::path::shaders_prefix + "skybox.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler_spherical.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    shader = load("shadow_map", {
        {GLShader::Vertex, lcf::path::shaders_prefix + "shadow_map.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "depth_map.frag"}, 
    });
    shader = load("animated_shadow_map", {
        {GLShader::Vertex, lcf::path::shaders_prefix + "animated_shadow_map.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "depth_map.frag"}, 
    });
    shader = load("depth_debug", {
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "depth_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
}

QString lcf::ShaderManager::readShaderSourceCode(const QString &file_path)
{
    QFile file(file_path);
    if (not file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "lcf::ShaderManager::readShaderSourceCode: Cannot open file:" << file_path;
        return "";
    }
    QTextStream in(&file);
    QString result;
    static QRegularExpression include_regex("#include\\s*\"([^\"]+)\"");
    while (not in.atEnd()) {
        QString line = in.readLine();
        auto match = include_regex.match(line);
        if (match.hasMatch()) {
            QString include_file_path = lcf::path::shaders_prefix + match.captured(1);
            result += readShaderSourceCode(include_file_path);
        } else {
            result += line + '\n';
        }
    }
    return result;
}