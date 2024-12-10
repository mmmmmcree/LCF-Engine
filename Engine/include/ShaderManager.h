#pragma once

#include "GLShaderProgram.h"
#include <unordered_map>
#include <memory>
#include <QObject>

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
            Skybox,
        };
        using ShaderInfo = std::pair<QOpenGLShader::ShaderTypeBit, QString>;
        using ShaderInfos = QList<ShaderInfo>;
        static ShaderManager *instance();
        void initialize();
        UniqueGLShaderProgramPtr load(const ShaderInfos &shader_infos);
        SharedGLShaderProgramPtr load(const QString &name, const ShaderInfos &shader_infos);
        SharedGLShaderProgramPtr get(const QString &name);
        SharedGLShaderProgramPtr get(ConfiguredShader type);
    private:
        ShaderManager();
    private:
        std::unordered_map<QString, SharedGLShaderProgramPtr> m_shaders;
    };
}