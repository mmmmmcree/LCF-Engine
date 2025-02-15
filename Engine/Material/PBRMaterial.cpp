#include "PBRMaterial.h"
#include "TextureManager.h"
#include "ControlManager.h"
#include "TextureDispatcher.h"

lcf::MaterialType lcf::PBRMaterial::type() const
{
    return lcf::MaterialType::PBR;
}

void lcf::PBRMaterial::setAlbedoMap(TextureWrapper texture)
{
    this->setTexture(TextureType::BaseColor, texture);
}

void lcf::PBRMaterial::setNormalMap(TextureWrapper texture)
{
    this->setTexture(TextureType::Normal, texture);
}

void lcf::PBRMaterial::setMetallicMap(TextureWrapper texture)
{
    this->setTexture(TextureType::Metalness, texture);
}

void lcf::PBRMaterial::setRoughnessMap(TextureWrapper texture)
{
    this->setTexture(TextureType::Roughness, texture);
}

void lcf::PBRMaterial::setAOMap(TextureWrapper texture)
{
    this->setTexture(TextureType::AmbientOcclusion, texture);
}

void lcf::PBRMaterial::setEmissiveMap(TextureWrapper texture)
{
    this->setTexture(TextureType::Emissive, texture);
}

lcf::PBRMaterial::PBRMaterial() : Material()
{
    auto tex_manager = TextureManager::instance();
    this->setAlbedoMap(tex_manager->get(TextureManager::DefaultAlbedo));
    this->setNormalMap(tex_manager->get(TextureManager::DefaultNormal));
    this->setMetallicMap(tex_manager->get(TextureManager::DefaultMetallic));
    this->setRoughnessMap(tex_manager->get(TextureManager::DefaultRoughness));
    this->setAOMap(tex_manager->get(TextureManager::DefaultAO));
    this->setEmissiveMap(tex_manager->get(TextureManager::DefaultEmissive));
}