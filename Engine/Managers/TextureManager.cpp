#include "TextureManager.h"
#include <QFileInfo>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "FrameBufferObject.h"
#include "ShaderManager.h"
#include "Geometry.h"
#include "GLHelper.h"

lcf::TextureManager *lcf::TextureManager::instance()
{
    static TextureManager s_instance;
    return &s_instance;
}

std::unique_ptr<lcf::GLTexture> lcf::TextureManager::load(const QString &image_path, GLTexture::TextureFormat internal_format, bool mirrored)
{
    auto loader = new ImageLoader(image_path, mirrored, Image::Format_RGBA8888, this);
    GLTexture *texture = new GLTexture(Image(1, 1, Image::Format_RGBA8888));
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
        
        auto gl = QOpenGLContext::currentContext()->functions();
        texture->setSize(image.width(), image.height());
        texture->setFormat(internal_format);
        texture->allocateStorage();
        texture->setData(GLTexture::RGBA, GLTexture::UInt8, image.constBits());

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

lcf::TextureWrapper lcf::TextureManager::fromSphereToCubeRGB(TextureWrapper source)
{
    const int width = 1024;
    bool current_context = QOpenGLContext::currentContext();
    if (not current_context) { m_context->makeCurrent(m_surface); }
    FrameBufferObject fbo(width, width);
    fbo.addColorAttachment(GLHelper::generateCubeMap(width));
    auto shader = ShaderManager::instance()->get(ShaderManager::SphereToCube);
    fbo.bind();
    shader->bind();
    source.bind(0);
    Geometry::cube()->draw();
    source.release(0);
    shader->release();
    fbo.release();
    if (not current_context) { m_context->doneCurrent(); }
    return TextureWrapper(fbo.colorAttachment());
}

lcf::TextureWrapper lcf::TextureManager::fromSphereToCubeRGBF(TextureWrapper source)
{
    const int width = 1024;
    bool current_context = QOpenGLContext::currentContext();
    if (not current_context) { m_context->makeCurrent(m_surface); }
    FrameBufferObject fbo(width, width);
    auto hdr_cube_map = GLHelper::generateHDRCubeMap(width);
    fbo.addColorAttachment(hdr_cube_map);
    auto shader = ShaderManager::instance()->get(ShaderManager::SphereToCube);
    fbo.bind();
    shader->bind();
    source.bind(0);
    Geometry::cube()->draw();
    source.release(0);
    shader->release();
    fbo.release();
    if (not current_context) { m_context->doneCurrent(); }
    return TextureWrapper(fbo.colorAttachment());    
}

lcf::TextureWrapper lcf::TextureManager::IBLConvolution(TextureWrapper environment_cubemap)
{
    const int width = 32;
    bool current_context = QOpenGLContext::currentContext();
    if (not current_context) { m_context->makeCurrent(m_surface); }
    FrameBufferObject fbo(width, width);
    fbo.addColorAttachment(GLHelper::generateHDRCubeMap(width));
    auto shader = ShaderManager::instance()->get(ShaderManager::IBLConvolution);
    fbo.bind();
    shader->bind();
    environment_cubemap.bind(0);
    Geometry::cube()->draw();
    auto gl = QOpenGLContext::currentContext()->functions();
    environment_cubemap.release(0);
    shader->release();
    fbo.release();
    if (not current_context) { m_context->doneCurrent(); }
    return TextureWrapper(fbo.colorAttachment());    
}

lcf::TextureWrapper lcf::TextureManager::loadCubeMap(const QString &right, const QString &left, const QString &top, const QString &bottom, const QString &front, const QString &back)
{
    QStringList paths = { right, left, top, bottom, front, back };
    auto gl = QOpenGLContext::currentContext()->functions();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    NativeTextureWrapper texture_wrapper(GLTexture::TargetCubeMap, texture);
    texture_wrapper.bind(0);
    for (int i = 0; i < 6; ++i) {
        LImage image(paths[i], false);
        gl->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGB, image.width(), image.height(), 0, static_cast<int>(image.format()), static_cast<int>(image.dataType()), image.data());
    }
    texture_wrapper.release(0);
    texture_wrapper.setWrapMode(GLTexture::ClampToEdge);
    texture_wrapper.setMinMagFilter(GLTexture::Linear, GLTexture::Linear);
    return texture_wrapper;
}

void lcf::TextureManager::initialize(QOpenGLContext * context)
{
    m_context = context;
    m_surface = new QOffscreenSurface(nullptr, this);
    m_surface->setFormat(context->format());
    m_surface->create();
}

lcf::SharedGLTexturePtr lcf::TextureManager::get(Type type)
{
    SharedGLTexturePtr texture;
    switch (type) {
        case DefaultDiffuse : { texture = m_textures["default_diffuse"]; } break;
        case DefaultSpecular : { texture = m_textures["default_specular"]; } break;
        case DefaultNormal : { texture = m_textures["default_normal"]; } break;
        case DefaultAlbedo : { texture = m_textures["default_diffuse"]; } break;
        case DefaultMetallic : { texture = m_textures["default_specular"]; } break;
        case DefaultRoughness : { texture = m_textures["default_specular"]; } break;
        case DefaultEmissive : { texture = m_textures["default_specular"]; } break;
        case DefaultAO : { texture = m_textures["default_specular"]; } break;
    }
    return texture;
}

lcf::TextureManager::TextureManager()
{
    Image image(1, 1, QImage::Format_RGBA8888);
    image.fill(Qt::white);
    SharedGLTexturePtr texture = std::make_shared<GLTexture>(image);
    texture->setWrapMode(GLTexture::ClampToEdge);
    m_textures.insert({"default_diffuse", texture});
    image.fill(Qt::black);
    texture = std::make_shared<GLTexture>(image);
    texture->setWrapMode(GLTexture::ClampToEdge);
    m_textures.insert({"default_specular", texture});
    image.fill(QColor(128, 128, 255));
    texture = std::make_shared<GLTexture>(image);
    texture->setWrapMode(GLTexture::ClampToEdge);
    m_textures.insert({"default_normal", texture});
}
