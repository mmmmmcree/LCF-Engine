#include "MaterialController.h"
#include "Define.h"

lcf::PhongMaterial::UniquePtr lcf::MaterialController::generatePhongMaterial()
{
    auto material = std::make_unique<lcf::PhongMaterial>();
    auto iter = m_textures.find(TextureType::Diffuse);
    if (iter != m_textures.end()) {
        material->setDiffuseMap(iter->second);
    }
    iter = m_textures.find(TextureType::Specular);
    if (iter != m_textures.end()) {
        material->setSpecularMap(iter->second);
    }
    iter = m_textures.find(TextureType::Normal);
    if (iter != m_textures.end()) {
        material->setNormalMap(iter->second);
    }
    material->setShininess(m_shininess);
    return material;
}

lcf::MaterialController::MaterialController()
{
}

void lcf::MaterialController::setMaterial(const MMaterial::SharedPtr &material)
{
    m_material = material;
}

void lcf::MaterialController::setTexture(int texture_type, TextureWrapper texture)
{
    auto iter = m_textures.find(texture_type);
    if (iter == m_textures.end()) {
        m_textures.emplace(std::make_pair(texture_type, texture));
    } else {
        iter->second = texture;
    }
}

const lcf::MaterialController::TextureInfoMap &lcf::MaterialController::textureInfoMap() const
{
    return m_textures;
}

void lcf::MaterialController::setTextures(const TextureInfoMap &texture_info_map)
{
    for (const auto & [type, texture] : texture_info_map) {
        this->setTexture(type, texture);
    }
}

void lcf::MaterialController::create()
{
    for (const auto & [type, image] : m_image_data) {
        auto iter = m_textures.find(type);
        if (iter != m_textures.end()) { continue; }
        m_textures.emplace(std::make_pair(type, std::make_shared<GLTexture>(image)));
    }
    m_image_data.clear();
    this->updateMaterial();
}

void lcf::MaterialController::bind()
{
    if (not m_material) { this->updateMaterial(); }
    m_material->bind();
}

void lcf::MaterialController::release()
{
    if (not m_material) { return; }
    m_material->release();
}

const lcf::UniformList &lcf::MaterialController::asUniformList() const
{
    return m_material->asUniformList();
}

void lcf::MaterialController::setType(Type type)
{
    m_type = type;
}

void lcf::MaterialController::setImageData(int texture_type, unsigned char *data, int width, int height)
{
    Image image = dataToImage(data, width, height);
    if (image.isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(texture_type, std::move(image)));
}

void lcf::MaterialController::setImageData(int texture_type, const Image &image)
{
    if (image.isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(texture_type, image));
}

void lcf::MaterialController::setImageData(int texture_type, Image &&image)
{
    if (image.isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(texture_type, std::move(image)));
}

void lcf::MaterialController::updateMaterial()
{
    switch (m_type) {
        case Phong : { m_material = this->generatePhongMaterial(); } break;
    }
}
