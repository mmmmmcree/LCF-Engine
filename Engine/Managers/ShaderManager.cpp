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

lcf::GLShaderProgram::UniquePtr lcf::ShaderManager::load(const ShaderInfos &shader_infos)
{
    GLShaderProgram *shader_program = new GLShaderProgram;
    for (const auto &[type, path] : shader_infos) {
        auto shader = this->getShader(type, path);
        if (not shader) { continue; }
        shader_program->addShader(shader);
    }
    if (not shader_program->link()) {
        delete shader_program; shader_program = nullptr;
    }
    return GLShaderProgram::UniquePtr(shader_program);
}

const lcf::GLShaderProgram::SharedPtr &lcf::ShaderManager::get(ConfiguredShader type) const
{
    return m_configured_shader_programs[type];
}

const lcf::GLShaderProgram::SharedPtr &lcf::ShaderManager::getShadowShader(LightType light_type, bool animated) const
{
    int index = 0;
    if (light_type == LightType::Directional) {
        index = ConfiguredShader::DirectionalShadowMap;
    } else if (light_type == LightType::Point) {
        index = ConfiguredShader::PointShadowMap;
    }
    return m_configured_shader_programs[index + animated];
}

const lcf::GLShaderProgram::SharedPtr &lcf::ShaderManager::getMaterialShader(MaterialType material_type, bool animated, bool shadowed) const
{
    int index = 0;
    switch (material_type) {
        case MaterialType::Phong : { index = ConfiguredShader::Phong; } break;
        case MaterialType::PBR : { index = ConfiguredShader::PBR; } break;
    }
    return m_configured_shader_programs[index + animated + shadowed * 2];
}

lcf::ShaderManager::ShaderManager() :
    QObject(),
    m_configured_shader_programs(ConfiguredShader::SIZE)
{
    GLShaderProgram::SharedPtr shader = nullptr;
    lcf::GLHelper::UniformInfos sampler_uniforms;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple2D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    m_configured_shader_programs[Simple2D] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler2D_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    m_configured_shader_programs[Simple3D] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "geometry_debug.vert"}, 
        {GLShader::Geometry, lcf::path::shaders_prefix + "geometry_debug.geom"},
        {GLShader::Fragment, lcf::path::shaders_prefix + "geometry_debug.frag"}, 
    });
    m_configured_shader_programs[GeometryDebug] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "single_color.frag"}, 
    });
    m_configured_shader_programs[SingleColor] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "skybox.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler_spherical.frag"}, 
    });
    m_configured_shader_programs[SphericalSkybox] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "skybox.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "sampler_cube.frag"}, 
    });
    m_configured_shader_programs[CubicSkybox] = shader;
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    shader = load({
        {GLShader::Vertex, path::shaders_prefix + "cube_map.vert"},
        {GLShader::Geometry, path::shaders_prefix + "cube_map.geom"},
        {GLShader::Fragment, path::shaders_prefix + "sphere_to_cube.frag"}
    });
    m_configured_shader_programs[SphereToCube] = shader;
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "directional_shadow_map.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "directional_shadow_map.frag"}, 
    });
    m_configured_shader_programs[DirectionalShadowMap] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "animated_directional_shadow_map.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "directional_shadow_map.frag"}, 
    });
    m_configured_shader_programs[AnimatedDirectionalShadowMap] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "point_shadow_map.vert"}, 
        {GLShader::Geometry, lcf::path::shaders_prefix + "point_shadow_map.geom"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "point_shadow_map.frag"}, 
    });
    m_configured_shader_programs[PointShadowMap] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "animated_point_shadow_map.vert"}, 
        {GLShader::Geometry, lcf::path::shaders_prefix + "point_shadow_map.geom"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "point_shadow_map.frag"}, 
    });
    m_configured_shader_programs[AnimatedPointShadowMap] = shader;
    shader = load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple3D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "depth_debug.frag"}, 
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    m_configured_shader_programs[DepthDebug] = shader;
    shader = load({
        {GLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    m_configured_shader_programs[Phong] = shader;
    shader = load({
        {GLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong.frag"},
    });
    m_configured_shader_programs[AnimatedPhong] = shader;
    shader = load({
        {GLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong_shadow.frag"},
    });
    m_configured_shader_programs[ShadowedPhong] = shader;
    shader = load({
        {GLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "phong_shadow.frag"},
    });
    m_configured_shader_programs[AnimatedShadowedPhong] = shader;
    shader = m_configured_shader_programs[PBR] = load({
        {GLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "pbr.frag"},
    });
    shader = m_configured_shader_programs[AnimatedPBR] = load({
        {GLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "pbr.frag"},
    });
    shader = m_configured_shader_programs[ShadowedPBR] = load({
        {GLShader::Vertex, path::shaders_prefix + "illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "pbr_shadow.frag"},
    });
    shader = m_configured_shader_programs[AnimatedShadowedPBR] = load({
        {GLShader::Vertex, path::shaders_prefix + "animated_illumination.vert"},
        {GLShader::Fragment, path::shaders_prefix + "pbr_shadow.frag"},
    });
    shader = m_configured_shader_programs[IBLConvolution] = load({
        {GLShader::Vertex, path::shaders_prefix + "cube_map.vert"},
        {GLShader::Geometry, path::shaders_prefix + "cube_map.geom"},
        {GLShader::Fragment, path::shaders_prefix + "IBL/convolution.frag"},
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    shader = m_configured_shader_programs[IBLPrefilter] = load({
        {GLShader::Vertex, path::shaders_prefix + "cube_map.vert"},
        {GLShader::Geometry, path::shaders_prefix + "cube_map.geom"},
        {GLShader::Fragment, path::shaders_prefix + "IBL/prefilter.frag"},
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
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

lcf::GLShader *lcf::ShaderManager::getShader(GLShader::ShaderTypeBit type, const QString &file_path)
{
    auto iter = m_shader_map.find(file_path);
    if (iter != m_shader_map.end()) {
        return iter->second;
    }
    GLShader *shader = new GLShader(type, this);
    if (not shader->compileSourceCode(this->readShaderSourceCode(file_path))) {
        qDebug() << shader->log();
        shader->deleteLater();
        return nullptr;
    }
    return m_shader_map[file_path] = shader;
}
