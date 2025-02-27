#include "GLHelper.h"
#include <QFileInfo>
#include <memory>
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
            shader->setUniformValue(name.toLocal8Bit(), value);
        }, uniform);
    }
    shader->release();
}

lcf::NativeTextureWrapper lcf::GLHelper::generateMSAATexture(int width, int height, int samples, GLTextureFormat format)
{
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    gl->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
    gl->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    NativeTextureWrapper texture_wrapper(GLTexture::Target2DMultisample, texture);
    return texture_wrapper;
}

lcf::NativeTextureWrapper lcf::GLHelper::generateCubeMapAttachment(int width, GLTexture::TextureFormat internal_format)
{
    int pixel_format = 0, pixel_data_type = 0;
    deducePixelFormatByTextureFormat(static_cast<GLTexture::TextureFormat>(internal_format), pixel_format, pixel_data_type);
    return generateCubeMap(width, internal_format, pixel_format, pixel_data_type);
}

lcf::NativeTextureWrapper lcf::GLHelper::generateCubeMap(int width, int internal_format, int pixel_format, int pixel_data_type, const void *data)
{
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    for (unsigned int i = 0; i < 6; i++) {
        gl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            internal_format, width, width, 0, pixel_format, pixel_data_type, data);
    }
    gl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    NativeTextureWrapper texture_wrapper(GLTexture::TargetCubeMap, texture);
    if (isDepthTextureFormat(static_cast<GLTexture::TextureFormat>(internal_format))) {
        texture_wrapper.setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
        texture_wrapper.setBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
        texture_wrapper.setWrapMode(GLTexture::ClampToBorder);
    } else {
        texture_wrapper.setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
        texture_wrapper.setWrapMode(GLTexture::ClampToEdge);
    }
    return texture_wrapper;
}

lcf::NativeTextureWrapper lcf::GLHelper::generateTexture2DAttachment(int width, int height, GLTexture::TextureFormat internal_format)
{
    int pixel_format = 0, pixel_data_type = 0;
    deducePixelFormatByTextureFormat(static_cast<GLTexture::TextureFormat>(internal_format), pixel_format, pixel_data_type);
    return generateTexture2D(width, height, internal_format, pixel_format, pixel_data_type);
}

lcf::NativeTextureWrapper lcf::GLHelper::generateTexture2D(int width, int height, int internal_format, int pixel_format, int pixel_type, const void *data)
{
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D, texture);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, pixel_format, pixel_type, data);
    gl->glBindTexture(GL_TEXTURE_2D, 0);
    NativeTextureWrapper texture_wrapper(GLTexture::Target2D, texture);
    if (isDepthTextureFormat(static_cast<GLTexture::TextureFormat>(internal_format))) {
        texture_wrapper.setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
        texture_wrapper.setBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
        texture_wrapper.setWrapMode(GLTexture::ClampToBorder);
    } else {
        texture_wrapper.setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
        texture_wrapper.setWrapMode(GLTexture::ClampToEdge);
    }
    return texture_wrapper;
}

lcf::NativeTextureWrapper lcf::GLHelper::generateTexture3D(int width, int height, int depth, int internal_format, int pixel_format, int pixel_type, const void *data)
{
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_3D, texture);
    gl->glTexImage3D(GL_TEXTURE_3D, 0, internal_format, width, height, depth, 0, pixel_format, pixel_type, data);
    gl->glBindTexture(GL_TEXTURE_3D, 0);
    NativeTextureWrapper texture_wrapper(GLTexture::Target3D, texture);
    if (isDepthTextureFormat(static_cast<GLTexture::TextureFormat>(internal_format))) {
        texture_wrapper.setMinMagFilters(GLTexture::Nearest, GLTexture::Nearest);
        texture_wrapper.setBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
        texture_wrapper.setWrapMode(GLTexture::ClampToBorder);
    } else {
        texture_wrapper.setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
        texture_wrapper.setWrapMode(GLTexture::ClampToEdge);
    }
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

lcf::SharedGLTexturePtr lcf::GLHelper::fromImageToTexture2D(const LImage & image, GLTexture::TextureFormat texture_format)
{
    auto tex = std::make_shared<GLTexture>(GLTexture::Target2D);
    tex->setSize(image.width(), image.height());
    tex->setFormat(texture_format);
    tex->allocateStorage();
    tex->setData(static_cast<GLTexture::PixelFormat>(image.format()), static_cast<GLTexture::PixelType>(image.dataType()), image.data());
    return tex;
}

lcf::NativeTextureWrapper lcf::GLHelper::fromImageToTexture3D(const LImage &image, GLTexture::TextureFormat texture_format)
{
    int dim = image.height();
    return GLHelper::generateTexture3D(dim, dim, dim, texture_format, image.format(), image.dataType(), image.data());
}

int lcf::GLHelper::maximumTextureUnits()
{
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    int max_texture_units;
    gl->glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
    return max_texture_units;
}

void lcf::GLHelper::deducePixelFormatByTextureFormat(GLTexture::TextureFormat texture_format, int &pixel_format, int &pixel_data_type)
{
    switch (texture_format) {
        case GLTexture::TextureFormat::D16 : {
            pixel_format = GL_DEPTH_COMPONENT;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::D24 : {
            pixel_format = GL_DEPTH_COMPONENT;
            pixel_data_type = GL_UNSIGNED_INT;
        } break;
        case GLTexture::TextureFormat::D32 : {
            pixel_format = GL_DEPTH_COMPONENT;
            pixel_data_type = GL_UNSIGNED_INT;
        } break;
        case GLTexture::TextureFormat::D24S8 : {
            pixel_format = GL_DEPTH_STENCIL;
            pixel_data_type = GL_UNSIGNED_INT_24_8;
        } break;
        case GLTexture::TextureFormat::D32F : {
            pixel_format = GL_DEPTH_COMPONENT;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::S8 : {
            pixel_format = GL_STENCIL_INDEX;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::R16F : {
            pixel_format = GL_RED;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::RG16F : {
            pixel_format = GL_RG;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::RGB16F : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::RGBA16F : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::R32F : {
            pixel_format = GL_RED;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::RG32F : {
            pixel_format = GL_RG;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::RGB32F : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::RGBA32F : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_FLOAT;
        } break;
        case GLTexture::TextureFormat::R8_UNorm : {
            pixel_format = GL_RED;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::RG8_UNorm : {
            pixel_format = GL_RG;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::RGB8_UNorm : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::RGBA8_UNorm : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::R16_UNorm : {
            pixel_format = GL_RED;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::RG16_UNorm : {
            pixel_format = GL_RG;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::RGB16_UNorm : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::RGBA16_UNorm : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::R8_SNorm : {
            pixel_format = GL_RED;
            pixel_data_type = GL_BYTE;
        } break;
        case GLTexture::TextureFormat::RG8_SNorm : {
            pixel_format = GL_RG;
            pixel_data_type = GL_BYTE;
        } break;
        case GLTexture::TextureFormat::RGB8_SNorm : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_BYTE;
        } break;
        case GLTexture::TextureFormat::RGBA8_SNorm : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_BYTE;
        } break;
        case GLTexture::TextureFormat::R16_SNorm : {
            pixel_format = GL_RED;
            pixel_data_type = GL_SHORT;
        } break;
        case GLTexture::TextureFormat::RG16_SNorm : {
            pixel_format = GL_RG;
            pixel_data_type = GL_SHORT;
        } break;
        case GLTexture::TextureFormat::RGB16_SNorm : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_SHORT;
        } break;
        case GLTexture::TextureFormat::RGBA16_SNorm : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_SHORT;
        } break;
        case GLTexture::TextureFormat::R8U : {
            pixel_format = GL_RED;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::RG8U : {
            pixel_format = GL_RG;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::RGB8U : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::RGBA8U : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
        case GLTexture::TextureFormat::R16U : {
            pixel_format = GL_RED;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::RG16U : {
            pixel_format = GL_RG;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::RGB16U : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::RGBA16U : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_UNSIGNED_SHORT;
        } break;
        case GLTexture::TextureFormat::R32U : {
            pixel_format = GL_RED;
            pixel_data_type = GL_UNSIGNED_INT;
        } break;
        case GLTexture::TextureFormat::RG32U : {
            pixel_format = GL_RG;
            pixel_data_type = GL_UNSIGNED_INT;
        } break;
        case GLTexture::TextureFormat::RGB32U : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_UNSIGNED_INT;
        } break;
        case GLTexture::TextureFormat::RGBA32U : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_UNSIGNED_INT;
        } break;
        case GLTexture::TextureFormat::R8I : {
            pixel_format = GL_RED;
            pixel_data_type = GL_BYTE;
        } break;
        case GLTexture::TextureFormat::RG8I : {
            pixel_format = GL_RG;
            pixel_data_type = GL_BYTE;
        } break;
        case GLTexture::TextureFormat::RGB8I : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_BYTE;
        } break;
        case GLTexture::TextureFormat::RGBA8I : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_BYTE;
        } break;
        case GLTexture::TextureFormat::R16I : {
            pixel_format = GL_RED;
            pixel_data_type = GL_SHORT;
        } break;
        case GLTexture::TextureFormat::RG16I : {
            pixel_format = GL_RG;
            pixel_data_type = GL_SHORT;
        } break;
        case GLTexture::TextureFormat::RGB16I : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_SHORT;
        } break;
        case GLTexture::TextureFormat::RGBA16I : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_SHORT;
        } break;
        case GLTexture::TextureFormat::R32I : {
            pixel_format = GL_RED;
            pixel_data_type = GL_INT;
        } break;
        case GLTexture::TextureFormat::RG32I : {
            pixel_format = GL_RG;
            pixel_data_type = GL_INT;
        } break;
        case GLTexture::TextureFormat::RGB32I : {
            pixel_format = GL_RGB;
            pixel_data_type = GL_INT;
        } break;
        case GLTexture::TextureFormat::RGBA32I : {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_INT;
        } break;
        default: {
            pixel_format = GL_RGBA;
            pixel_data_type = GL_UNSIGNED_BYTE;
        } break;
    }
}

lcf::GLTexture::TextureFormat lcf::GLHelper::deduceTextureFormatByPixelFormat(int pixel_format, int pixel_data_type, bool SRGB)
{
    GLTextureFormat texture_format = GLTextureFormat::NoFormat;
    if (pixel_format == GL_DEPTH_COMPONENT and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::D16;
    } else if (pixel_format == GL_DEPTH_COMPONENT and pixel_data_type == GL_UNSIGNED_INT) {
        texture_format = GLTextureFormat::D24;
    } else if (pixel_format == GL_DEPTH_COMPONENT and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::D32F;
    } else if (pixel_format == GL_STENCIL_INDEX and pixel_data_type == GL_UNSIGNED_BYTE) {
        texture_format = GLTextureFormat::S8;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::R16F;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::RG16F;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::RGB16F;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::RGBA16F;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::R32F;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::RG32F;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::RGB32F;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_FLOAT) {
        texture_format = GLTextureFormat::RGBA32F;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_UNSIGNED_BYTE) {
        texture_format = GLTextureFormat::R8_UNorm;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_UNSIGNED_BYTE) {
        texture_format = GLTextureFormat::RG8_UNorm;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_UNSIGNED_BYTE) {
        if (SRGB) { texture_format = GLTextureFormat::SRGB8; }
        else { texture_format = GLTextureFormat::RGB8_UNorm; }
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_UNSIGNED_BYTE) {
        if (SRGB) { texture_format = GLTextureFormat::SRGB8_Alpha8; }
        else { texture_format = GLTextureFormat::RGBA8_UNorm; }
    } else if (pixel_format == GL_RED and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::R16_UNorm;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::RG16_UNorm;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::RGB16_UNorm;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::RGBA16_UNorm;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_BYTE) {
        texture_format = GLTextureFormat::R8_SNorm;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_BYTE) {
        texture_format = GLTextureFormat::RG8_SNorm;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_BYTE) {
        texture_format = GLTextureFormat::RGB8_SNorm;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_BYTE) {
        texture_format = GLTextureFormat::RGBA8_SNorm;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_SHORT) {
        texture_format = GLTextureFormat::R16_SNorm;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_SHORT) {
        texture_format = GLTextureFormat::RG16_SNorm;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_SHORT) {
        texture_format = GLTextureFormat::RGB16_SNorm;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_SHORT) {
        texture_format = GLTextureFormat::RGBA16_SNorm;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_UNSIGNED_BYTE) {
        texture_format = GLTextureFormat::R8U;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_UNSIGNED_BYTE) {
        texture_format = GLTextureFormat::RG8U;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_UNSIGNED_BYTE) {
        texture_format = GLTextureFormat::RGB8U;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_UNSIGNED_BYTE) {
        texture_format = GLTextureFormat::RGBA8U;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::R16U;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::RG16U;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::RGB16U;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_UNSIGNED_SHORT) {
        texture_format = GLTextureFormat::RGBA16U;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_UNSIGNED_INT) {
        texture_format = GLTextureFormat::R32U;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_UNSIGNED_INT) {
        texture_format = GLTextureFormat::RG32U;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_UNSIGNED_INT) {
        texture_format = GLTextureFormat::RGB32U;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_UNSIGNED_INT) {
        texture_format = GLTextureFormat::RGBA32U;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_BYTE) {
        texture_format = GLTextureFormat::R8I;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_BYTE) {
        texture_format = GLTextureFormat::RG8I;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_BYTE) {
        texture_format = GLTextureFormat::RGB8I;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_BYTE) {
        texture_format = GLTextureFormat::RGBA8I;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_SHORT) {
        texture_format = GLTextureFormat::R16I;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_SHORT) {
        texture_format = GLTextureFormat::RG16I;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_SHORT) {
        texture_format = GLTextureFormat::RGB16I;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_SHORT) {
        texture_format = GLTextureFormat::RGBA16I;
    } else if (pixel_format == GL_RED and pixel_data_type == GL_INT) {
        texture_format = GLTextureFormat::R32I;
    } else if (pixel_format == GL_RG and pixel_data_type == GL_INT) {
        texture_format = GLTextureFormat::RG32I;
    } else if (pixel_format == GL_RGB and pixel_data_type == GL_INT) {
        texture_format = GLTextureFormat::RGB32I;
    } else if (pixel_format == GL_RGBA and pixel_data_type == GL_INT) {
        texture_format = GLTextureFormat::RGBA32I;
    } else {
        texture_format = GLTextureFormat::RGBA8_UNorm;
    }
    return texture_format;
}

bool lcf::GLHelper::isFloatingPointTextureFormat(GLTexture::TextureFormat texture_format)
{
    return texture_format == GLTexture::TextureFormat::R16F or
        texture_format == GLTexture::TextureFormat::RG16F or
        texture_format == GLTexture::TextureFormat::RGB16F or
        texture_format == GLTexture::TextureFormat::RGBA16F or
        texture_format == GLTexture::TextureFormat::R32F or
        texture_format == GLTexture::TextureFormat::RG32F or
        texture_format == GLTexture::TextureFormat::RGB32F or
        texture_format == GLTexture::TextureFormat::RGBA32F;
}

bool lcf::GLHelper::isDepthTextureFormat(GLTextureFormat texture_format)
{
    return texture_format == GLTextureFormat::D16 or
        texture_format == GLTextureFormat::D24 or
        texture_format == GLTextureFormat::D24S8 or
        texture_format == GLTextureFormat::D32 or
        texture_format == GLTextureFormat::D32F or
        texture_format == GLTextureFormat::D32FS8X24 or
        texture_format == GLTextureFormat::S8;
}

bool lcf::GLHelper::isSampler(int uniform_type)
{
    return uniform_type == GL_SAMPLER_2D or
        uniform_type == GL_SAMPLER_CUBE or
        uniform_type == GL_SAMPLER_3D or
        uniform_type == GL_SAMPLER_2D_SHADOW or
        uniform_type == GL_SAMPLER_2D_ARRAY or
        uniform_type == GL_SAMPLER_CUBE_MAP_ARRAY;
}
