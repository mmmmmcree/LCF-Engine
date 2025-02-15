#include "IBLMaterial.h"
#include "TextureDispatcher.h"

lcf::MaterialType lcf::IBLMaterial::type() const
{
    return MaterialType::IBL;
}

void lcf::IBLMaterial::setIrradianceMap(TextureWrapper texture)
{
    this->setTexture(TextureType::IBLIrradiance, texture);
}

void lcf::IBLMaterial::setPrefilterMap(TextureWrapper texture)
{
    this->setTexture(TextureType::IBLPrefilter, texture);
}

void lcf::IBLMaterial::setBRDFMap(TextureWrapper texture)
{
    this->setTexture(TextureType::IBLBRDF, texture);
}

lcf::IBLMaterial::UniquePtr lcf::IBLMaterial::createUnique()
{
    return UniquePtr(new IBLMaterial);
}

lcf::IBLMaterial::IBLMaterial() : Material()
{
}
