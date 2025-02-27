#include "TextureWrapper.h"

lcf::TextureWrapper::TextureWrapper(GLTexture *texture)
{
    if (texture and texture->textureId()) { m_texture = texture; }
}

lcf::TextureWrapper::TextureWrapper(const std::shared_ptr<GLTexture> &texture)
{
    if (texture and texture->textureId()) { m_texture = texture; }
}

lcf::TextureWrapper::TextureWrapper(std::unique_ptr<GLTexture> &&texture)
{
    if (texture and texture->textureId()) { m_texture = std::move(texture); }
}

lcf::TextureWrapper::TextureWrapper(const NativeTextureWrapper &texture)
{
    if (texture.textureId()) {
        m_texture = texture;
    }
}

lcf::TextureWrapper *lcf::TextureWrapper::operator->()
{
    return this;
}

void lcf::TextureWrapper::bind(unsigned int unit)
{
    if (not m_texture.has_value()) { return; }
    std::visit([unit](auto&& arg) {
        arg->bind(unit);
    }, m_texture.value());
}

void lcf::TextureWrapper::release(unsigned int unit)
{
    if (not m_texture.has_value()) { return; }
    std::visit([unit](auto&& arg) {
        arg->release(unit);
    }, m_texture.value());
}

bool lcf::TextureWrapper::isValid()
{
    bool valid = false;
    if (not m_texture.has_value()) { return valid; }
    std::visit([&valid](auto&& arg) {
        valid = arg->textureId();
    }, m_texture.value());
    return valid;
}

int lcf::TextureWrapper::width()
{
    int width = 0;
    if (not m_texture.has_value()) { return width; }
    std::visit([&width](auto&& arg) {
        width = arg->width();
    }, m_texture.value());
    return width;
}

int lcf::TextureWrapper::height()
{
    int height = 0;
    if (not m_texture.has_value()) { return height; }
    std::visit([&height](auto&& arg) {
        height = arg->height();
    }, m_texture.value());
    return height;
}

int lcf::TextureWrapper::depth()
{
    int depth = 0;
    if (not m_texture.has_value()) { return depth; }
    std::visit([&depth](auto&& arg) {    
        depth = arg->depth();
    }, m_texture.value());
    return depth;
}

lcf::GLTexture::Target lcf::TextureWrapper::target()
{
    GLTexture::Target target = GLTexture::Target::Target2D;
    if (not this->isValid()) { return target; }
    std::visit([&target](auto&& arg) {
        target = arg->target();
    }, m_texture.value());
    return target;
}

lcf::GLTexture::TextureFormat lcf::TextureWrapper::format()
{
    GLTexture::TextureFormat format = GLTexture::TextureFormat::NoFormat;
    if (not this->isValid()) { return format; }
    std::visit([&format](auto&& arg) {
        format = arg->format();
    }, m_texture.value());
    return format;
}

void lcf::TextureWrapper::setWrapMode(GLTexture::WrapMode wrap_mode)
{
    if (not this->isValid()) { return; }
    std::visit([wrap_mode](auto&& arg) {
        arg->setWrapMode(wrap_mode);
    }, m_texture.value());
}

void lcf::TextureWrapper::setMinMagFilters(GLTexture::Filter min_filter, GLTexture::Filter mag_filter)
{
    if (not this->isValid()) { return; }
    std::visit([min_filter, mag_filter](auto&& arg) {
        arg->setMinMagFilters(min_filter, mag_filter);
    }, m_texture.value());
}

void lcf::TextureWrapper::setBorderColor(float r, float g, float b, float a)
{
    if (not this->isValid()) { return; }
    std::visit([r, g, b, a](auto&& arg) {
        arg->setBorderColor(r, g, b, a);
    }, m_texture.value());
}

void lcf::TextureWrapper::generateMipMaps()
{
    if (not this->isValid()) { return; }
    std::visit([](auto&& arg) {
        arg->generateMipMaps();
    }, m_texture.value());
}
