#pragma once

#include "GLTexture.h"
#include <unordered_map>
#include "Image.h"


namespace lcf {
    class TextureManager : public QObject
    {
        Q_OBJECT
    public:
        static TextureManager *instance();
        std::unique_ptr<GLTexture> load(const QString &image_path);
        std::unique_ptr<GLTexture> load(unsigned char *data, int width, int height);
    private:
        TextureManager() = default;
    };
}