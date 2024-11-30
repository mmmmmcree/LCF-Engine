#include "TextureManager.h"


lcf::TextureManager *lcf::TextureManager::instance()
{
    static TextureManager s_instance;
    return &s_instance;
}

std::unique_ptr<lcf::GLTexture> lcf::TextureManager::load(const QString &image_path)
{
    std::unique_ptr<GLTexture> texture = std::make_unique<GLTexture>(QImage(1, 1, QImage::Format_RGBA8888));
    auto loader = new ImageLoader(image_path, this);
    connect(loader, &ImageLoader::imageLoaded, this, [texture = texture.get()](const QImage &image) {
        texture->destroy();
        texture->setData(image);
    });
    connect(loader, &ImageLoader::finished, loader, &QObject::deleteLater);
    loader->start();
    return texture;
}

std::unique_ptr<lcf::GLTexture> lcf::TextureManager::load(unsigned char *data, int width, int height)
{
    return std::make_unique<GLTexture>(dataToImage(data, width, height));
}

