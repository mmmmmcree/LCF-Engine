#pragma once

#include "GLTexture.h"
#include <unordered_map>
#include "Image.h"
#include <QOpenGLContext>
#include <QOffscreenSurface>


namespace lcf {
    class TextureManager : public QObject
    {
        Q_OBJECT
    public:
        static TextureManager *instance();
        std::unique_ptr<GLTexture> load(const QString &image_path, bool mirroed = false);
        std::unique_ptr<GLTexture> loadSingleThread(unsigned char *data, int width, int height);
        std::unique_ptr<GLTexture> loadSingleThread(const QString &image_path, bool mirrored = false);
        void initialize(QOpenGLContext *context);
    private:
        TextureManager() = default;
    private:
        QOpenGLContext *m_context = nullptr;
        QOffscreenSurface *m_surface = nullptr;
    };
}