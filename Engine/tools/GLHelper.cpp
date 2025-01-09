#include "GLHelper.h"
#include <QFileInfo>
#include <memory>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "GLFunctions.h"


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

lcf::NativeTextureWrapper lcf::GLHelper::generateColorTexture(int width, int height, GLTextureFormat internal_format)
{
    auto gl = QOpenGLContext::currentContext()->functions();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D, texture);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    gl->glBindTexture(GL_TEXTURE_2D, 0);
    NativeTextureWrapper texture_wrapper(texture);
    texture_wrapper.setTarget(GL_TEXTURE_2D);
    return texture_wrapper;
}

lcf::NativeTextureWrapper lcf::GLHelper::generateDepthStencilTexture(int width, int height)
{
    auto gl = QOpenGLContext::currentContext()->functions();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D, texture);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    gl->glBindTexture(GL_TEXTURE_2D, 0);
    NativeTextureWrapper texture_wrapper(texture);
    texture_wrapper.setTarget(GL_TEXTURE_2D);
    return texture_wrapper;
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

lcf::NativeTextureWrapper lcf::GLHelper::generateMSAATexture(int width, int height, int samples, GLTextureFormat format)
{
    auto gl = GLFunctions::global();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    gl->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_TRUE);
    gl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    NativeTextureWrapper texture_wrapper(texture);
    texture_wrapper.setTarget(GL_TEXTURE_2D_MULTISAMPLE);
    return texture_wrapper;
}

lcf::NativeTextureWrapper lcf::GLHelper::generateFloatingPointTexture(int width, int height)
{
    auto gl = QOpenGLContext::currentContext()->functions();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D, texture);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    gl->glBindTexture(GL_TEXTURE_2D, 0);
    NativeTextureWrapper texture_wrapper(texture);
    texture_wrapper.setTarget(GL_TEXTURE_2D);
    return texture_wrapper;
}

lcf::SharedGLTexturePtr lcf::GLHelper::generateTextureByTextureType(TextureType type, const Image &image)
{
    return generateTextureByTextureType(type, image.width(), image.height(), image.constBits());
}

lcf::SharedGLTexturePtr lcf::GLHelper::generateTextureByTextureType(TextureType type, int width, int height, const unsigned char *data)
{
    auto texture = std::make_shared<GLTexture>(GLTexture::Target2D);
    texture->create();
    texture->setSize(width, height);
    if (type == Diffuse or type == Emissive or type == BaseColor or type == EmissionColor) {
        texture->setFormat(GLTexture::SRGB8_Alpha8);
        texture->setMinMagFilters(GLTexture::LinearMipMapLinear, GLTexture::LinearMipMapLinear);
        texture->generateMipMaps();
    } else {
        texture->setFormat(GLTexture::RGBA8_UNorm);
        texture->setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
    }
    texture->allocateStorage();
    texture->setData(GLTexture::RGBA, QOpenGLTexture::UInt8, data);
    return texture;
}

int lcf::GLHelper::maximumTextureUnits()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    int max_texture_units;
    gl->glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
    return max_texture_units;
}
