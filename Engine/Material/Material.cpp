#include "Material.h"
#include "TextureManager.h"

lcf::Material::Material(const Material &other)
{
    if (not other.m_created) {
        qDebug() << "Material not created! You will receive a blank material.";
        return;
    }
    m_textures = other.m_textures;
}

void lcf::Material::addTexture(TextureWrapper texture)
{
    if (not m_created) { return; }
    m_textures.push_back(texture);
}

void lcf::Material::addData(unsigned char *data, int width, int height)
{
    if (m_created) {
        qDebug() << "Material already created! Cannot add texture data.";
        return;
    }
    Image image = dataToImage(data, width, height);
    if (image.isNull()) { return; }
    m_image_data.emplace_back(std::move(image));
}

void lcf::Material::addImageData(const Image &image)
{
    if (m_created) {
        qDebug() << "Material already created! Cannot add texture data.";
        return;
    }
    if (image.isNull()) { return; }
    m_image_data.emplace_back(image);
}

void lcf::Material::addImageData(Image &&image)
{
    if (m_created) {
        qDebug() << "Material already created! Cannot add texture data.";
        return;
    }
    if (image.isNull()) { return; }
    m_image_data.emplace_back(std::move(image));
}

void lcf::Material::create()
{
    if (m_created) { return; }
    m_created = true;
    for (const auto &image : m_image_data) {
        qDebug() << "Material: " << "Creating texture from image data.";
        m_textures.emplace_back(std::make_unique<GLTexture>(image));
    }
    m_image_data.clear();
}

void lcf::Material::bind()
{
    for (int i = 0; i < m_textures.size(); ++i) {
        m_textures[i].bind(i);
    }
}

void lcf::Material::release()
{
    for (int i = 0; i < m_textures.size(); ++i) {
        m_textures[i].release();
    }
}
