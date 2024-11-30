#include "TextureWrapper.h"
#include "ShaderToys/ShaderToy.h"

lcf::TextureWrapper::TextureWrapper(GLTexture *texture) : m_texture(texture)
{
}

lcf::TextureWrapper::TextureWrapper(ShaderToy *texture) : m_texture(texture)
{
}

lcf::TextureWrapper::TextureWrapper(const std::shared_ptr<GLTexture> &texture) :
    m_texture(texture)
{
}

lcf::TextureWrapper::TextureWrapper(std::unique_ptr<GLTexture> &&texture) :
    m_texture(std::shared_ptr<GLTexture>(std::move(texture)))
{
}

void lcf::TextureWrapper::bind(unsigned int unit)
{
    std::visit([unit](auto&& arg) {
        arg->bind(unit);
    }, m_texture);
}

void lcf::TextureWrapper::release()
{
    std::visit([](auto&& arg) {
        arg->release();
    }, m_texture);
}
