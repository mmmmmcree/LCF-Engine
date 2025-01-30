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
            SIZE
        };
        using ShaderInfo = std::pair<GLShader::ShaderTypeBit, QString>;
        using ShaderInfos = QList<ShaderInfo>;
        static ShaderManager *instance();
        UniqueGLShaderProgramPtr load(const ShaderInfos &shader_infos);
        SharedGLShaderProgramPtr load(const QString &name, const ShaderInfos &shader_infos);
        SharedGLShaderProgramPtr get(const QString &name);
        const SharedGLShaderProgramPtr &get(ConfiguredShader type) const;
        const SharedGLShaderProgramPtr &getShadowShader(LightType light_type, bool animated) const;
        const SharedGLShaderProgramPtr &getMaterialShader(MaterialType material_type, bool animated, bool shadowed) const;
    private:
        ShaderManager();
    public:
        QString readShaderSourceCode(const QString &file_path); // 给shader文件添加include功能
        QString _readShaderSourceCode(const QString &file_path, QSet<QString> &included_files); // 给shader文件添加include功能
    private:
        std::unordered_map<QString, SharedGLShaderProgramPtr> m_shader_map;
        std::vector<SharedGLShaderProgramPtr> m_configured_shaders;
    };
}