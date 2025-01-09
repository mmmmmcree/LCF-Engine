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

lcf::UniqueGLShaderProgramPtr lcf::ShaderManager::load(const ShaderInfos &shader_infos)
{
    GLShaderProgram *shader = new GLShaderProgram;
    for (const auto &[type, path] : shader_infos) {
        shader->addShaderFromSourceCode(type, this->readShaderSourceCode(path));
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
    if (m_shader_map.find(name) != m_shader_map.end()) {
        qDebug() << "Shader already loaded: " << name;
    } else {
        m_shader_map.emplace(name, load(shader_infos));
    }
    return m_shader_map[name];
}

lcf::SharedGLShaderProgramPtr lcf::ShaderManager::get(const QString &name)
{
    auto iter = m_shader_map.find(name);
    if (iter != m_shader_map.end()) { return iter->second; }
    return nullptr;
}

const lcf::SharedGLShaderProgramPtr &lcf::ShaderManager::get(ConfiguredShader type) const
{
    return m_configured_shaders[type];
}

const lcf::SharedGLShaderProgramPtr &lcf::ShaderManager::getShadowShader(LightType light_type, bool animated) const
{
    int index = 0;
    if (light_type == LightType::Directional) {
        index = ConfiguredShader::DirectionalShadowMap;
    } else if (light_type == LightType::Point) {
        index = ConfiguredShader::PointShadowMap;
    }
    return m_configured_shaders[index + animated];
}

lcf::ShaderManager::ShaderManager() :
    QObject(),
    m_configured_shaders(ConfiguredShader::SIZE)
{
    SharedGLShaderProgramPtr shader = nullptr;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple2D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    m_configured_shaders[Simple2D] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple2D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "post_process.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    m_configured_shaders[PostProcess] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    m_configured_shaders[Simple3D] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "geometry_debug.vert"}, 
        {GLShader::Geometry, lcf::path::shaders_prefix + "geometry_debug.geom"},
        {GLShader::Fragment, lcf::path::shaders_prefix + "geometry_debug.frag"}, 
    });
    m_configured_shaders[GeometryDebug] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "single_color.frag"}, 
    });
    m_configured_shaders[SingleColor] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "skybox.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler_spherical.frag"}, 
    });
    m_configured_shaders[Skybox] = shader;
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "directional_shadow_map.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "directional_shadow_map.frag"}, 
    });
    m_configured_shaders[DirectionalShadowMap] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "animated_directional_shadow_map.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "directional_shadow_map.frag"}, 
    });
    m_configured_shaders[AnimatedDirectionalShadowMap] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "point_shadow_map.vert"}, 
        {GLShader::Geometry, lcf::path::shaders_prefix + "point_shadow_map.geom"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "point_shadow_map.frag"}, 
    });
    m_configured_shaders[PointShadowMap] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "animated_point_shadow_map.vert"}, 
        {GLShader::Geometry, lcf::path::shaders_prefix + "point_shadow_map.geom"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "point_shadow_map.frag"}, 
    });
    m_configured_shaders[AnimatedPointShadowMap] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "depth_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    m_configured_shaders[DepthDebug] = shader;
}

QString lcf::ShaderManager::readShaderSourceCode(const QString & file_path)
{
    QSet<QString> included_files;
    return this->_readShaderSourceCode(file_path, included_files);
}

QString lcf::ShaderManager::_readShaderSourceCode(const QString &file_path, QSet<QString> &included_files)
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
        if (not match.hasMatch()) {
            result += line + '\n';
            continue;
        }
        QString include_file_name = match.captured(1);
        if (included_files.contains(include_file_name)) { continue; }
        else { included_files.insert(include_file_name); }
        QString include_file_path = lcf::path::shaders_prefix + "include/" + include_file_name;
        result += _readShaderSourceCode(include_file_path, included_files);
    }
    return result;
}