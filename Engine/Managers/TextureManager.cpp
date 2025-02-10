#include "TextureManager.h"
#include <QFileInfo>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "FrameBufferObject.h"
#include "ShaderManager.h"
#include "Geometry.h"
#include "GLHelper.h"
#include "LImage.h"
#include "Constants.h"

lcf::TextureManager *lcf::TextureManager::instance()
{
    static TextureManager s_instance;
    return &s_instance;
}

lcf::SharedGLTexturePtr lcf::TextureManager::loadTexture2D(const QString &image_path, bool SRGB, bool mirrored)
{
    bool current_context = QOpenGLContext::currentContext();
    if (not current_context) { m_context->makeCurrent(m_surface); }
    LImage image(image_path, mirrored);
    int format = static_cast<int>(image.format());
    int data_type = static_cast<int>(image.dataType());
    return GLHelper::fromImageToTexture(image, GLHelper::deduceTextureFormatByPixelFormat(format, data_type, SRGB));
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
    texture_wrapper.setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
    return texture_wrapper;
}

void lcf::TextureManager::initialize(QOpenGLContext * context)
{
    m_context = context;
    m_surface = context->surface();
}

lcf::SharedGLTexturePtr lcf::TextureManager::get(Type type)
{
    SharedGLTexturePtr texture;
    switch (type) {
        case DefaultDiffuse : { texture = m_textures["default_diffuse"]; } break;
        case DefaultSpecular : { texture = m_textures["default_specular"]; } break;
        case DefaultNormal : { texture = m_textures["default_normal"]; } break;
        case DefaultAmbient : { texture = m_textures["default_ambient"]; } break;
        case DefaultAlbedo : { texture = m_textures["default_diffuse"]; } break;
        case DefaultMetallic : { texture = m_textures["default_specular"]; } break;
        case DefaultRoughness : { texture = m_textures["default_specular"]; } break;
        case DefaultEmissive : { texture = m_textures["default_specular"]; } break;
        case DefaultAO : { texture = m_textures["default_diffuse"]; } break;
    }
    return texture;
}

lcf::TextureManager::TextureManager()
{
    Image image(1, 1, QImage::Format_RGBA8888);
    image.fill({255, 255, 255});
    SharedGLTexturePtr texture = std::make_shared<GLTexture>(image);
    texture->setWrapMode(GLTexture::ClampToEdge);
    m_textures.insert({"default_diffuse", texture});
    image.fill({0, 0, 0});
    texture = std::make_shared<GLTexture>(image);
    texture->setWrapMode(GLTexture::ClampToEdge);
    m_textures.insert({"default_specular", texture});
    image.fill(QColor(128, 128, 255));
    texture = std::make_shared<GLTexture>(image);
    texture->setWrapMode(GLTexture::ClampToEdge);
    m_textures.insert({"default_normal", texture});
    image.fill(QColor(10, 10, 10));
    texture = std::make_shared<GLTexture>(image);
    texture->setWrapMode(GLTexture::ClampToEdge);
    m_textures.insert({"default_ambient", texture});
}
