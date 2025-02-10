#pragma once

#include "GLTexture.h"
#include <unordered_map>
#include "Image.h"
#include <QOpenGLContext>
#include <QSurface>
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
            DefaultAmbient,
            DefaultAlbedo,
            DefaultEmissive,
            DefaultRoughness,
            DefaultMetallic,
            DefaultAO,
        };
        static TextureManager *instance();
        SharedGLTexturePtr loadTexture2D(const QString &image_path, bool SRGB, bool mirrored = true);

        TextureWrapper loadCubeMap(const QString &right, const QString &left, const QString &top, const QString &bottom, const QString &front, const QString &back);
        void initialize(QOpenGLContext *context);
        SharedGLTexturePtr get(Type type);
    private:
        TextureManager();
    private:
        QOpenGLContext *m_context = nullptr;
        QSurface *m_surface = nullptr;
        std::unordered_map<QString, SharedGLTexturePtr> m_textures;
    };
}