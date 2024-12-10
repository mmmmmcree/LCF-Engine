#include "Material.h"
#include "TextureManager.h"
#include <QOpenGLFunctions>

lcf::Material::Material(const Material &other)
{
    m_textures = other.m_textures;
}

lcf::Material::SharedPtr lcf::Material::newShared()
{
    return std::make_shared<Material>();
}

void lcf::Material::setTexture(int texture_type, TextureWrapper texture)
{
    auto iter = m_textures.find(texture_type);
    if (iter == m_textures.end()) {
        m_textures.emplace(std::make_pair(texture_type, texture));
    } else {
        iter->second = texture;
    }
}

const lcf::Material::TextureInfoMap &lcf::Material::textureInfoMap() const
{
    return m_textures;
}

void lcf::Material::setTextures(const TextureInfoMap & texture_info_map)
{
    for (const auto & [type, texture] : texture_info_map) {
        this->setTexture(type, texture);
    }
}

void lcf::Material::setImageData(int texture_type, unsigned char *data, int width, int height)
{
    Image image = dataToImage(data, width, height);
    if (image.isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(texture_type, std::move(image)));
}

void lcf::Material::setImageData(int texture_type, const Image &image)
{
    if (image.isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(texture_type, image));
}

void lcf::Material::setImageData(int texture_type, Image &&image)
{
    if (image.isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(texture_type, std::move(image)));
}

void lcf::Material::create()
{
    //! 不会覆盖已有的纹理
    for (const auto & [type, image] : m_image_data) {
        auto iter = m_textures.find(type);
        if (iter != m_textures.end()) { continue; }
        m_textures.emplace(std::make_pair(type, std::make_shared<GLTexture>(image)));
    }
    m_image_data.clear();
}

void lcf::Material::bind()
{
    int unit = 0;
    for (auto &[type, texture] : m_textures) {
        texture.bind(unit++);
    }
}

void lcf::Material::release()
{
    for (auto &[type, texture] : m_textures) {
        texture.release();
    }
}