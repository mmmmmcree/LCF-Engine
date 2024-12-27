#include "MaterialController.h"
#include "Define.h"
#include "GLHelper.h"

lcf::PhongMaterial::UniquePtr lcf::MaterialController::generatePhongMaterial()
{
    auto material = PhongMaterial::UniquePtr(new lcf::PhongMaterial);
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

lcf::PBRMaterial::UniquePtr lcf::MaterialController::generatePBRMaterial()
{
    auto material = PBRMaterial::UniquePtr(new lcf::PBRMaterial);
    auto iter = m_textures.find(TextureType::BaseColor);
    if (iter != m_textures.end()) {
        material->setAlbedoMap(iter->second);
        qDebug() << "setAlbedoMap";
    }
    iter = m_textures.find(TextureType::Metalness);
    if (iter != m_textures.end()) {
        material->setMetallicMap(iter->second);
        qDebug() << "setMetallicMap";
    }
    iter = m_textures.find(TextureType::Roughness);
    if (iter != m_textures.end()) {
        material->setRoughnessMap(iter->second);
        qDebug() << "setRoughnessMap";
    }
    iter = m_textures.find(TextureType::Normal);
    if (iter != m_textures.end()) {
        material->setNormalMap(iter->second);
        qDebug() << "setNormalMap";
    }
    iter = m_textures.find(TextureType::Emissive);
    if (iter != m_textures.end()) {
        material->setEmissiveMap(iter->second);
        qDebug() << "setEmissiveMap";
    }
    iter = m_textures.find(TextureType::AmbientOcclusion);
    if (iter != m_textures.end()) {
        material->setAOMap(iter->second);
        qDebug() << "setAOMap";
    }
    return material;
}

lcf::UserCustomMaterial::UniquePtr lcf::MaterialController::generateUserCustomMaterial()
{
    auto material = UserCustomMaterial::UniquePtr(new lcf::UserCustomMaterial(&m_textures));
    return material;
}

lcf::MaterialController::SharedPtr lcf::MaterialController::createShared()
{
    return std::make_shared<lcf::MaterialController>();
}

const lcf::Material::SharedPtr & lcf::MaterialController::material() const
{
    return m_material;
}

void lcf::MaterialController::setTexture(int texture_type, TextureWrapper texture)
{
    auto iter = m_textures.find(texture_type);
    if (iter == m_textures.end()) {
        m_textures.emplace(std::make_pair(texture_type, texture));
    } else {
        iter->second = texture;
    }
    this->updateMaterial();
}

const lcf::MaterialController::TextureInfoMap &lcf::MaterialController::textureInfoMap() const
{
    return m_textures;
}

void lcf::MaterialController::setTextures(const TextureInfoMap &texture_info_map)
{
    for (const auto & [type, texture] : texture_info_map) {
        auto iter = m_textures.find(type);
        if (iter != m_textures.end()) {
            iter->second = texture;
        } else {
            m_textures.emplace(std::make_pair(type, texture));
        }
    }
    this->updateMaterial();
}

void lcf::MaterialController::create()
{
    for (const auto & [type, image] : m_image_data) {
        auto iter = m_textures.find(type);
        if (iter != m_textures.end()) { continue; }
        auto texture = GLHelper::generateTextureByTextureType(static_cast<TextureType>(type), *image);
        m_textures.emplace(std::make_pair(type, texture));
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
    static const lcf::UniformList empty_list;
    if (not m_material) { return empty_list; }
    return m_material->asUniformList();
}

void lcf::MaterialController::setType(MaterialType type)
{
    m_material_type = type;
    this->updateMaterial();
}

lcf::MaterialType lcf::MaterialController::materialType() const
{
    return m_material_type;
}

void lcf::MaterialController::setShininess(float shininess)
{
    m_shininess = std::max(shininess, std::numeric_limits<float>::min()); //! shinness <= 0.0f在计算镜面反射出现bug
}

void lcf::MaterialController::setImageData(int type, const SharedImagePtr &image)
{
    if (not image or image->isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(type, image));
}

// void lcf::MaterialController::setImageData(int texture_type, unsigned char *data, int width, int height)
// {
//     Image image = dataToImage(data, width, height);
//     if (image.isNull()) { return; }
//     m_image_data.emplace_back(std::make_pair(texture_type, std::move(image)));
// }

// void lcf::MaterialController::setImageData(int texture_type, const Image &image)
// {
//     if (image.isNull()) { return; }
//     m_image_data.emplace_back(std::make_pair(texture_type, image));
// }

// void lcf::MaterialController::setImageData(int texture_type, Image &&image)
// {
//     if (image.isNull()) { return; }
//     m_image_data.emplace_back(std::make_pair(texture_type, std::move(image)));
// }

void lcf::MaterialController::updateMaterial()
{
    switch (m_material_type) {
        case Phong : { m_material = this->generatePhongMaterial(); } break;
        case UserCustom : { m_material = this->generateUserCustomMaterial(); } break;
        case PBR : { m_material = this->generatePBRMaterial(); } break;
    }
}
