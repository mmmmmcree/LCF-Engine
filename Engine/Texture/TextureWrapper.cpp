#include "TextureWrapper.h"

lcf::TextureWrapper::TextureWrapper(GLTexture *texture)
{
    if (texture) { m_texture = texture; }
}

lcf::TextureWrapper::TextureWrapper(ShaderToy *texture)
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

lcf::TextureWrapper::TextureWrapper(unsigned int texture)
{
    if (texture) { m_texture = NativeTextureWrapper(texture); }
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

void lcf::TextureWrapper::release()
{
    if (not m_texture.has_value()) { return; }
    std::visit([](auto&& arg) {
        arg->release();
    }, m_texture.value());
}

bool lcf::TextureWrapper::isValid() const
{
    return m_texture.has_value();
}
