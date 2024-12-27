#include "PBRMaterial.h"
#include "TextureManager.h"

lcf::MaterialType lcf::PBRMaterial::type() const
{
    return lcf::MaterialType::PBR;
}

void lcf::PBRMaterial::setAlbedoMap(TextureWrapper texture)
{
    m_textures[0] = texture;
}

void lcf::PBRMaterial::setNormalMap(TextureWrapper texture)
{
    m_textures[1] = texture;
}

void lcf::PBRMaterial::setMetallicMap(TextureWrapper texture)
{
    m_textures[2] = texture;
}

void lcf::PBRMaterial::setRoughnessMap(TextureWrapper texture)
{
    m_textures[3] = texture;
}

void lcf::PBRMaterial::setAOMap(TextureWrapper texture)
{
    m_textures[4] = texture;
}

void lcf::PBRMaterial::setEmissiveMap(TextureWrapper texture)
{
    m_textures[5] = texture;
}

lcf::PBRMaterial::PBRMaterial() : Material()
{
    m_textures.resize(6);
    m_uniforms.emplace_back(SingleUniform("material.albedo_map", [] { return 0; }));
    m_uniforms.emplace_back(SingleUniform("material.normal_map", 1));
    m_uniforms.emplace_back(SingleUniform("material.metallic_map", 2));
    m_uniforms.emplace_back(SingleUniform("material.roughness_map", 3));
    m_uniforms.emplace_back(SingleUniform("material.ao_map", 4));
    m_uniforms.emplace_back(SingleUniform("material.emissive_map", 5));
    auto tex_manager = TextureManager::instance();
    this->setAlbedoMap(tex_manager->get(TextureManager::DefaultAlbedo));
    this->setNormalMap(tex_manager->get(TextureManager::DefaultNormal));
    this->setMetallicMap(tex_manager->get(TextureManager::DefaultMetallic));
    this->setRoughnessMap(tex_manager->get(TextureManager::DefaultRoughness));
    this->setAOMap(tex_manager->get(TextureManager::DefaultAO));
    this->setEmissiveMap(tex_manager->get(TextureManager::DefaultEmissive));
}