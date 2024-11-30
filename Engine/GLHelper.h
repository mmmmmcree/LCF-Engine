#pragma once

#include <QOpenGLShaderProgram>
#include <variant>
#include <QList>
#include <QVector4D>
#include <QColor>
#include <unordered_map>
#include <vector>
#include <memory>


struct GLHelper
{
    using Uniform = std::variant<int, float, QVector2D, QVector3D, QColor, QVector4D, QMatrix2x2, QMatrix3x3, QMatrix4x4>;
    using UniformInfo = std::pair<QString, Uniform>;
    using UniformInfos = std::initializer_list<UniformInfo>;
    using ShaderInfo = std::pair<QOpenGLShader::ShaderTypeBit, QString>;
    using ShaderInfos = QList<ShaderInfo>;
    // 匿名则不可用getShader获取
    // static QString defaultShaderPath(const QString &file_name = "");
    // static std::unique_ptr<QOpenGLShaderProgram> loadShaderWithPaths(const ShaderInfos &shader_infos);
    // static QOpenGLShaderProgram *loadShaderWithPaths(const QString &name, const ShaderInfos &shader_infos);
    // static std::unique_ptr<QOpenGLShaderProgram> loadShaderWithFileNames(ShaderInfos shader_infos, QString path = "");
    // static QOpenGLShaderProgram *loadShaderWithFileNames(const QString &name, ShaderInfos shader_infos, QString path = "");
    static void setShaderUniform(QOpenGLShaderProgram * shader, const UniformInfo &uniform_info);
    static void setShaderUniforms(QOpenGLShaderProgram * shader, const UniformInfos &uniform_infos);
    // static QOpenGLShaderProgram *getShader(const QString &name);
// private:
    // inline static std::unordered_map<QString, std::unique_ptr<QOpenGLShaderProgram>> s_shaders;
};