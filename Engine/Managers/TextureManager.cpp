#include "TextureManager.h"
#include <QFileInfo>

lcf::TextureManager *lcf::TextureManager::instance()
{
    static TextureManager s_instance;
    return &s_instance;
}

std::unique_ptr<lcf::GLTexture> lcf::TextureManager::load(const QString &image_path, bool mirrored)
{
    auto loader = new ImageLoader(image_path, this);
    GLTexture *texture = new GLTexture(QImage(1, 1, QImage::Format_RGBA8888));
    if (not QFileInfo(image_path).exists()) {
        qDebug() << "lcf::TextureManager::load: file not found: " << image_path;
    }
    connect(loader, &ImageLoader::imageLoaded, this, [=](const QImage &image) {
        m_context->makeCurrent(m_surface);
        auto wrap_mode_r = texture->wrapMode(GLTexture::DirectionR);
        auto wrap_mode_s = texture->wrapMode(GLTexture::DirectionS);
        auto wrap_mode_t = texture->wrapMode(GLTexture::DirectionT);
        auto filters = texture->minMagFilters();
        texture->destroy();
        texture->setData(mirrored ? image.mirrored() : image);
        texture->setWrapMode(GLTexture::DirectionR, wrap_mode_r);
        texture->setWrapMode(GLTexture::DirectionS, wrap_mode_s);
        texture->setWrapMode(GLTexture::DirectionT, wrap_mode_t);
        texture->setMinMagFilters(filters.first, filters.second);
        m_context->doneCurrent();
    });
    connect(loader, &ImageLoader::finished, loader, &QObject::deleteLater);
    loader->start();
    return std::unique_ptr<GLTexture>(texture);
}

std::unique_ptr<lcf::GLTexture> lcf::TextureManager::loadSingleThread(unsigned char *data, int width, int height)
{
    return std::make_unique<GLTexture>(dataToImage(data, width, height));
}

std::unique_ptr<lcf::GLTexture> lcf::TextureManager::loadSingleThread(const QString & image_path, bool mirrored)
{
    return std::make_unique<GLTexture>(mirrored ? QImage(image_path).mirrored() : QImage(image_path));
}

void lcf::TextureManager::initialize(QOpenGLContext * context)
{
    m_context = context;
    m_surface = new QOffscreenSurface(nullptr, this);
    m_surface->setFormat(context->format());
    m_surface->create();
}

