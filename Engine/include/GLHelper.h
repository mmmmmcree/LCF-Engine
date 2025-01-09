#pragma once

#include <QOpenGLShaderProgram>
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


namespace lcf {
    struct GLHelper
    {
        using Uniform = std::variant<int, float, QVector2D, QVector3D, QColor, QVector4D, QMatrix2x2, QMatrix3x3, QMatrix4x4>;
        using UniformInfo = std::pair<QString, Uniform>;
        using UniformInfos = std::initializer_list<UniformInfo>;

        static void setShaderUniform(QOpenGLShaderProgram * shader, const UniformInfo &uniform_info);
        static void setShaderUniforms(QOpenGLShaderProgram * shader, const UniformInfos &uniform_infos);

        static NativeTextureWrapper generateColorTexture(int width, int height, GLTextureFormat internal_format);
        static NativeTextureWrapper generateDepthStencilTexture(int width, int height);
        static NativeTextureWrapper generateDepthMap(int width, int height);
        static NativeTextureWrapper generateCubeDepthMap(int width, int height);
        static NativeTextureWrapper generateMSAATexture(int width, int height, int samples, GLTextureFormat format);
        static NativeTextureWrapper generateFloatingPointTexture(int width, int height);

        static SharedGLTexturePtr generateTextureByTextureType(TextureType type, const Image &image);
        static SharedGLTexturePtr generateTextureByTextureType(TextureType type, int width, int height, const unsigned char *data);

        static int maximumTextureUnits();
    };
}