#pragma once

#include "GLShaderProgram.h"
#include <variant>
#include <QList>
#include <QVector4D>
#include <QColor>
#include <unordered_map>
#include <vector>
#include <memory>
#include "NativeTextureWrapper.h"
#include "Define.h"
#include "GLTexture.h"
#include "Image.h"
#include "LImage.h"
#include "GLMVector.h"
#include "GLMMatrix.h"


namespace lcf {
    struct GLHelper
    {
        using Uniform = std::variant<int, float, QVector2D, QVector3D, QColor, QVector4D, QMatrix2x2, QMatrix3x3, QMatrix4x4,
            GLMVector3D, GLMVector4D, GLMMatrix3x3, GLMMatrix4x4>;
        using UniformInfo = std::pair<QString, Uniform>;
        using UniformInfos = std::vector<UniformInfo>;

        static void setShaderUniform(GLShaderProgram * shader, const UniformInfo &uniform_info);
        static void setShaderUniforms(GLShaderProgram * shader, const UniformInfos &uniform_infos);


        static NativeTextureWrapper generateMSAATexture(int width, int height, int samples, GLTextureFormat format);

        static NativeTextureWrapper generateCubeMapAttachment(int width, GLTexture::TextureFormat internal_format);
        static NativeTextureWrapper generateCubeMap(int width, int internal_format, int pixel_format, int pixel_type, const void *data = nullptr);

        static NativeTextureWrapper generateTexture2DAttachment(int width, int height, GLTexture::TextureFormat internal_format);
        static NativeTextureWrapper generateTexture2D(int width, int height, int internal_format, int pixel_format, int pixel_type, const void *data = nullptr);

        static NativeTextureWrapper generateTexture3D(int width, int height, int depth, int internal_format, int pixel_format, int pixel_type, const void *data = nullptr);

        static SharedGLTexturePtr generateTextureByTextureType(TextureType type, const Image &image);
        static SharedGLTexturePtr generateTextureByTextureType(TextureType type, int width, int height, const unsigned char *data);

        static SharedGLTexturePtr fromImageToTexture2D(const LImage &image, GLTexture::TextureFormat texture_format);
        static NativeTextureWrapper fromImageToTexture3D(const LImage &image, GLTexture::TextureFormat texture_format);

        static int maximumTextureUnits();
        static void deducePixelFormatByTextureFormat(GLTexture::TextureFormat texture_format, int &pixel_format, int &pixel_data_type);
        static GLTexture::TextureFormat deduceTextureFormatByPixelFormat(int pixel_format, int pixel_data_type, bool SRGB);
        static bool isFloatingPointTextureFormat(GLTexture::TextureFormat texture_format);
        static bool isDepthTextureFormat(GLTextureFormat texture_format);
        static bool isSampler(int uniform_type);
    private:
        static NativeTextureWrapper generateTexture(int target, int width, int height, int depth, int internal_format, int pixel_format, int pixel_type, const void *data = nullptr);
    };
}