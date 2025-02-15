#pragma once

#include "GLShaderProgram.h"
#include <unordered_map>
#include <memory>
#include <QObject>
#include "Define.h"
#include "Model.h"

namespace lcf {
    class ShaderManager : public QObject
    {
        Q_OBJECT
    public:
        enum ConfiguredShader
        {
            Simple2D,
            Simple3D,
            GeometryDebug,
            SingleColor,
            SphericalSkybox,
            CubicSkybox,
            SphereToCube,
            DirectionalShadowMap,
            AnimatedDirectionalShadowMap,
            PointShadowMap,
            AnimatedPointShadowMap,
            DepthDebug,
            Phong,
            AnimatedPhong,
            ShadowedPhong,
            AnimatedShadowedPhong,
            PBR,
            AnimatedPBR,
            ShadowedPBR,
            AnimatedShadowedPBR,
            IBLConvolution,
            IBLPrefilter,
            SIZE
        };
        using ShaderInfo = std::pair<GLShader::ShaderTypeBit, QString>;
        using ShaderInfos = QList<ShaderInfo>;
        static ShaderManager *instance();
        GLShaderProgram::UniquePtr load(const ShaderInfos &shader_infos);
        const GLShaderProgram::SharedPtr &get(ConfiguredShader type) const;
        const GLShaderProgram::SharedPtr &getShadowShader(LightType light_type, bool animated) const;
        const GLShaderProgram::SharedPtr &getMaterialShader(MaterialType material_type, bool animated, bool shadowed) const;
    private:
        ShaderManager();
    public:
        QString readShaderSourceCode(const QString &file_path); // 给shader文件添加include功能
        QString _readShaderSourceCode(const QString &file_path, QSet<QString> &included_files); // 给shader文件添加include功能
        GLShader *getShader(GLShader::ShaderTypeBit type, const QString &file_path);
    private:
        std::unordered_map<QString, GLShader *> m_shader_map;
        std::vector<GLShaderProgram::SharedPtr> m_configured_shader_programs;
    };
}