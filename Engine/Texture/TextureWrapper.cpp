#include "TextureWrapper.h"

lcf::TextureWrapper::TextureWrapper(GLTexture *texture)
{
    if (texture) { m_texture = texture; }
}

lcf::TextureWrapper::TextureWrapper(const std::shared_ptr<GLTexture> &texture)
{
    if (texture) { m_texture = texture; }
}

lcf::TextureWrapper::TextureWrapper(std::unique_ptr<GLTexture> &&texture)
{
    if (texture) { m_texture = std::move(texture); }
}

lcf::TextureWrapper::TextureWrapper(const NativeTextureWrapper &texture)
{
    if (texture.id()) {
        m_texture = texture;
    }
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

bool lcf::TextureWrapper::isValid() const
{
    return m_texture.has_value();
}

int lcf::TextureWrapper::target()
{
    int target = 0;
    if (not this->isValid()) { return target; }
    std::visit([&target](auto&& arg) {
        target = arg->target();
    }, m_texture.value());
    return target;
}

int lcf::TextureWrapper::format()
{
    int format = GLTexture::TextureFormat::NoFormat;
    if (not this->isValid()) { return format; }
    std::visit([&format](auto&& arg) {
        format = arg->format();
    }, m_texture.value());
    return format;
}

void lcf::TextureWrapper::setWrapMode(GLTexture::WrapMode wrap_mode)
{
    if (not m_texture.has_value()) { return; }
    std::visit([wrap_mode](auto&& arg) {
        arg->setWrapMode(wrap_mode);
    }, m_texture.value());
}
