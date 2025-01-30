#include "NativeTextureWrapper.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>


lcf::NativeTextureWrapper::~NativeTextureWrapper()
{
    if (not m_info or --m_info->m_ref_count > 0) { return; }
    if (QOpenGLContext::currentContext() and QOpenGLContext::currentContext()->isValid()) {
        auto gl = QOpenGLContext::currentContext()->functions();
        gl->glDeleteTextures(1, &m_info->texture);
    }
    delete m_info; m_info = nullptr;
}

lcf::NativeTextureWrapper::NativeTextureWrapper(GLTexture::Target target, unsigned int texture, bool managed)
{
    if (not texture) {
        qDebug() << "Invalid texture";
        return;
    }
    m_info = new TextureInfo();
    m_info->texture = texture;
    m_info->target = target;
    if (managed) { m_info->m_ref_count = 1; } 
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    this->bind(0);
    gl->glGetTexLevelParameteriv(m_info->target, 0, GL_TEXTURE_WIDTH, &m_info->width);
    gl->glGetTexLevelParameteriv(m_info->target, 0, GL_TEXTURE_HEIGHT, &m_info->height);
    gl->glGetTexLevelParameteriv(m_info->target, 0, GL_TEXTURE_INTERNAL_FORMAT, &m_info->format);
    this->release(0);

}

lcf::NativeTextureWrapper::NativeTextureWrapper(const NativeTextureWrapper &other) :
    m_info(other.m_info)
{
    if (m_info) { ++m_info->m_ref_count; }
}

lcf::NativeTextureWrapper *lcf::NativeTextureWrapper::operator->()
{
    return this;
}

lcf::NativeTextureWrapper &lcf::NativeTextureWrapper::operator=(const NativeTextureWrapper &other)
{
    if (m_info and --m_info->m_ref_count == 0) {
        if (QOpenGLContext::currentContext() and QOpenGLContext::currentContext()->isValid()) {
            auto gl = QOpenGLContext::currentContext()->functions();
            gl->glDeleteTextures(1, &m_info->texture);
        }
        delete m_info; m_info = nullptr;
    }
    m_info = other.m_info;
    ++m_info->m_ref_count;
    return *this;
}

lcf::GLTexture::Target lcf::NativeTextureWrapper::target() const
{
    if (not m_info) { return GLTexture::Target2D; }
    return static_cast<GLTexture::Target>(m_info->target);
}

void lcf::NativeTextureWrapper::bind(int unit) const
{
    if (not this->isValid()) { return; }
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0 + unit);
    gl->glBindTexture(m_info->target, m_info->texture);
}

void lcf::NativeTextureWrapper::release(int unit) const
{
    if (not this->isValid()) { return; }
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0 + unit);
    gl->glBindTexture(m_info->target, 0);
}

unsigned int lcf::NativeTextureWrapper::id() const
{
    if (not m_info) { return 0; }
    return m_info->texture;
}

bool lcf::NativeTextureWrapper::isValid() const
{
    return m_info;
}

std::pair<int, int> lcf::NativeTextureWrapper::size() const
{
    if (not m_info) { return std::make_pair(0, 0); }
    return std::make_pair(m_info->width, m_info->height);
}

int lcf::NativeTextureWrapper::width() const
{
    if (not m_info) { return 0; }
    return m_info->width;
}

int lcf::NativeTextureWrapper::height() const
{
    if (not m_info) { return 0; }
    return m_info->height;
}

lcf::GLTexture::TextureFormat lcf::NativeTextureWrapper::format() const
{
    if (not m_info) { return GLTexture::TextureFormat::NoFormat; }
    return static_cast<GLTexture::TextureFormat>(m_info->format);
}

void lcf::NativeTextureWrapper::setWrapMode(GLTexture::WrapMode wrap_mode)
{
    if (not this->isValid()) { return; }
    auto gl = QOpenGLContext::currentContext()->functions();
    this->bind(0);
    gl->glTexParameteri(m_info->target, GL_TEXTURE_WRAP_S, wrap_mode);
    gl->glTexParameteri(m_info->target, GL_TEXTURE_WRAP_T, wrap_mode);
    gl->glTexParameteri(m_info->target, GL_TEXTURE_WRAP_R, wrap_mode);
    this->release(0);
}

void lcf::NativeTextureWrapper::setMinMagFilter(GLTexture::Filter min_filter, GLTexture::Filter mag_filter)
{
    if (not this->isValid()) { return; }
    auto gl = QOpenGLContext::currentContext()->functions();
    this->bind(0);
    gl->glTexParameteri(m_info->target, GL_TEXTURE_MIN_FILTER, min_filter);
    gl->glTexParameteri(m_info->target, GL_TEXTURE_MAG_FILTER, mag_filter);
    this->release(0);
}
