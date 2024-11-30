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
        using ShaderInfo = std::pair<QOpenGLShader::ShaderTypeBit, QString>;
        using ShaderInfos = QList<ShaderInfo>;
        static ShaderManager *instance();
        void initialize();
        std::unique_ptr<GLShaderProgram> load(const ShaderInfos &shader_infos);
        GLShaderProgram *load(const QString &name, const ShaderInfos &shader_infos);
        GLShaderProgram *get(const QString &name);
        const QString &defaultPath();
    private:
        ShaderManager();
    private:
        std::unordered_map<QString, std::unique_ptr<GLShaderProgram>> m_shaders;
    };
}