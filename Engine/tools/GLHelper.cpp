#include "GLHelper.h"
#include <QFileInfo>
#include <memory>
#include <QOpenGLContext>
#include <QOpenGLFunctions>


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

lcf::NativeTextureWrapper lcf::GLHelper::generateDepthMap(int width, int height)
{
    auto gl = QOpenGLContext::currentContext()->functions();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D, texture);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
        width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    gl->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    gl->glBindTexture(GL_TEXTURE_2D, 0);
    NativeTextureWrapper texture_wrapper(texture);
    texture_wrapper.setTarget(GL_TEXTURE_2D);
    return texture_wrapper;
}

lcf::NativeTextureWrapper lcf::GLHelper::generateCubeDepthMap(int width, int height)
{
    auto gl = QOpenGLContext::currentContext()->functions();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    for (unsigned int i = 0; i < 6; i++) {
        gl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }
    gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    gl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    NativeTextureWrapper texture_wrapper(texture);
    texture_wrapper.setTarget(GL_TEXTURE_CUBE_MAP);
    return texture_wrapper;
}
