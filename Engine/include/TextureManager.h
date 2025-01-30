#pragma once

#include "GLTexture.h"
#include <unordered_map>
#include "Image.h"
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include "TextureWrapper.h"


namespace lcf {
    class TextureManager : public QObject
    {
        Q_OBJECT
    public:
        enum Type
        {
            DefaultDiffuse,
            DefaultNormal,
            DefaultSpecular,
            DefaultAlbedo,
            DefaultEmissive,
            DefaultRoughness,
            DefaultMetallic,
            DefaultAO,
        };
        static TextureManager *instance();
        std::unique_ptr<GLTexture> load(const QString &image_path, GLTexture::TextureFormat internal_format = GLTexture::SRGB8_Alpha8, bool mirrored = true);
        std::unique_ptr<GLTexture> loadSingleThread(unsigned char *data, int width, int height);
        std::unique_ptr<GLTexture> loadSingleThread(const QString &image_path, bool mirrored = false);
        TextureWrapper fromSphereToCubeRGB(TextureWrapper source);
        TextureWrapper fromSphereToCubeRGBF(TextureWrapper source);
        TextureWrapper IBLConvolution(TextureWrapper environment_cubemap);
        TextureWrapper loadCubeMap(const QString &right, const QString &left, const QString &top, const QString &bottom, const QString &front, const QString &back);
        void initialize(QOpenGLContext *context);
        SharedGLTexturePtr get(Type type);
    private:
        TextureManager();
    private:
        QOpenGLContext *m_context = nullptr;
        QOffscreenSurface *m_surface = nullptr;
        std::unordered_map<QString, SharedGLTexturePtr> m_textures;
    };
}