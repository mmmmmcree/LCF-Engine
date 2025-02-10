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
    m_texture_map["material.albedo_map"] = texture;
}

void lcf::PBRMaterial::setNormalMap(TextureWrapper texture)
{
    m_texture_map["material.normal_map"] = texture;
}

void lcf::PBRMaterial::setMetallicMap(TextureWrapper texture)
{
    m_texture_map["material.metallic_map"] = texture;
}

void lcf::PBRMaterial::setRoughnessMap(TextureWrapper texture)
{
    m_texture_map["material.roughness_map"] = texture;
}

void lcf::PBRMaterial::setAOMap(TextureWrapper texture)
{
    m_texture_map["material.ao_map"] = texture;
}

void lcf::PBRMaterial::setEmissiveMap(TextureWrapper texture)
{
    m_texture_map["material.emissive_map"] = texture;
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
    // int location = 0;
    // m_uniforms.emplace_back(SingleUniform("material.albedo_map", location++));
    // m_uniforms.emplace_back(SingleUniform("material.normal_map", location++));
    // m_uniforms.emplace_back(SingleUniform("material.metallic_map", location++));
    // m_uniforms.emplace_back(SingleUniform("material.roughness_map", location++));
    // m_uniforms.emplace_back(SingleUniform("material.ao_map", location++));
    // m_uniforms.emplace_back(SingleUniform("material.emissive_map", location++));
    // m_uniforms.emplace_back(SingleUniform("ibl_material.irradiance_map", location++));
    // m_uniforms.emplace_back(SingleUniform("ibl_material.prefilter_map", location++));
    // m_uniforms.emplace_back(SingleUniform("ibl_material.brdf_map", location++));
}