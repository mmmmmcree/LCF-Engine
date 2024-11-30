#include "GLHelper.h"
#include <QFileInfo>
#include <memory>

// QString GLHelper::defaultShaderPath(const QString &file_name)
// {
//     static QString s_default_path = QString(SOURCE_DIR) + QString("/shaders/");
//     return s_default_path + file_name;
// }

// std::unique_ptr<QOpenGLShaderProgram> GLHelper::loadShaderWithPaths(const ShaderInfos &shader_infos)
// {
//     QOpenGLShaderProgram *shader = new QOpenGLShaderProgram();
//     for (const auto &[type, path] : shader_infos) {
//         if (QFileInfo(path).exists()) {
//             shader->addShaderFromSourceFile(type, path);
//         } else {
//             qDebug() << "Shader file not found: " << path;
//         }
//     }
//     if (not shader->link()) {
//         qDebug() << shader->log();
//         delete shader;
//         shader = nullptr;
//     }
//     return std::unique_ptr<QOpenGLShaderProgram>(shader);
// }

// // QOpenGLShaderProgram *GLHelper::loadShaderWithPaths(const QString &name, const ShaderInfos &shader_infos)
// // {
// //     if (s_shaders.find(name) != s_shaders.end()) {
// //         qDebug() << "Shader already loaded: " << name;
// //     } else {
// //         s_shaders.emplace(name, loadShaderWithPaths(shader_infos));
// //     }
// //     return s_shaders[name].get();
// // }

// std::unique_ptr<QOpenGLShaderProgram> GLHelper::loadShaderWithFileNames(ShaderInfos shader_infos, QString path)
// {
//     if (path.isEmpty()) { path = defaultShaderPath(); }
//     for (auto &[type, file_name] : shader_infos) {
//         file_name = path + file_name;
//     }
//     return loadShaderWithPaths(shader_infos);
// }

// QOpenGLShaderProgram *GLHelper::loadShaderWithFileNames(const QString &name, ShaderInfos shader_infos, QString path)
// {
//     if (s_shaders.find(name) != s_shaders.end()) {
//         qDebug() << "Shader already loaded: " << name;
//     } else {
//         s_shaders.emplace(name, loadShaderWithFileNames(shader_infos));
//     }
//     return s_shaders[name].get();
// }

void GLHelper::setShaderUniform(QOpenGLShaderProgram *shader, const UniformInfo &uniform)
{
    shader->bind();
    std::visit([&](auto &&value) {
        shader->setUniformValue(uniform.first.toStdString().c_str(), value);
    }, uniform.second);
    shader->release();
}

void GLHelper::setShaderUniforms(QOpenGLShaderProgram *shader, const UniformInfos &uniform_infos)
{
    shader->bind();
    for (const auto &[name, uniform] : uniform_infos) {
        std::visit([&](auto &&value) {
            shader->setUniformValue(name.toStdString().c_str(), value);
        }, uniform);
    }
    shader->release();
}

// QOpenGLShaderProgram *GLHelper::getShader(const QString &name)
// {
//     auto iter = s_shaders.find(name);
//     if (iter != s_shaders.end()) { return iter->second.get(); }
//     return nullptr;
// }
