#include "Material.h"
#include "TextureManager.h"
#include <QOpenGLFunctions>

lcf::Material::Material(const Material &other)
{
    if (not other.m_created) {
        qDebug() << "Material not created! You will receive a blank material.";
        return;
    }
    m_textures = other.m_textures;
}

lcf::Material::SharedPtr lcf::Material::newShared()
{
    return std::make_shared<Material>();
}

lcf::Material::SharedPtr lcf::Material::newCreatedShared()
{
    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->create();
    return material;
}

void lcf::Material::addTexture(TextureWrapper texture)
{
    if (not m_created) { return; }
    m_textures.emplace_back(texture);
}

void lcf::Material::addTextures(const TextureList &textures)
{
    m_textures.insert(m_textures.end(), textures.begin(), textures.end());
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

const lcf::Material::TextureList &lcf::Material::textures() const
{
    return m_textures;
}

void lcf::Material::setTexture(int index, TextureWrapper texture)
{
    if (index < 0 or index >= m_textures.size()) { return; }
    m_textures[index] = texture;
}

void lcf::Material::create()
{
    if (m_created) { return; }
    m_created = true;
    for (const auto &image : m_image_data) {
        m_textures.emplace_back(std::make_unique<GLTexture>(image));
    }
    m_image_data.clear();
}

bool lcf::Material::isCreated() const
{
    return m_created;
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
    auto gl = QOpenGLContext::currentContext()->functions();
}
