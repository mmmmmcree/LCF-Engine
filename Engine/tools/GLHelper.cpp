#include "GLHelper.h"
#include <QFileInfo>
#include <memory>


void lcf::GLHelper::setShaderUniform(QOpenGLShaderProgram *shader, const UniformInfo &uniform)
{
    shader->bind();
    std::visit([&](auto &&value) {
        shader->setUniformValue(uniform.first.toStdString().c_str(), value);
    }, uniform.second);
    shader->release();
}

void lcf::GLHelper::setShaderUniforms(QOpenGLShaderProgram *shader, const UniformInfos &uniform_infos)
{
    shader->bind();
    for (const auto &[name, uniform] : uniform_infos) {
        std::visit([&](auto &&value) {
            shader->setUniformValue(name.toStdString().c_str(), value);
        }, uniform);
    }
    shader->release();
}

