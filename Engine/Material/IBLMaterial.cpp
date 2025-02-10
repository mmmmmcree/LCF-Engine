#include "IBLMaterial.h"
#include "TextureDispatcher.h"

lcf::MaterialType lcf::IBLMaterial::type() const
{
    return MaterialType::IBL;
}

void lcf::IBLMaterial::setIrradianceMap(TextureWrapper texture)
{
    m_texture_map["ibl_material.irradiance_map"] = texture;
}

void lcf::IBLMaterial::setPrefilterMap(TextureWrapper texture)
{
    m_texture_map["ibl_material.prefilter_map"] = texture;
}

void lcf::IBLMaterial::setBRDFMap(TextureWrapper texture)
{
    m_texture_map["ibl_material.brdf_map"] = texture;
}

lcf::IBLMaterial::UniquePtr lcf::IBLMaterial::createUnique()
{
    return UniquePtr(new IBLMaterial);
}

lcf::IBLMaterial::IBLMaterial() : Material()
{
}
