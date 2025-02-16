#include "PhongMaterial.h"
#include "TextureManager.h"

lcf::PhongMaterial::PhongMaterial() :
    Material()
{
    m_shininess.setName("material.shininess");
    auto texture_manager = TextureManager::instance();
    this->setDiffuseMap(texture_manager->get(TextureManager::DefaultDiffuse));
    this->setSpecularMap(texture_manager->get(TextureManager::DefaultSpecular));
    this->setNormalMap(texture_manager->get(TextureManager::DefaultNormal));
}

lcf::MaterialType lcf::PhongMaterial::type() const
{
    return MaterialType::Phong;
}

void lcf::PhongMaterial::setDiffuseMap(TextureWrapper texture)
{
    this->setTexture(TextureType::Diffuse, texture);
}
    
void lcf::PhongMaterial::setSpecularMap(TextureWrapper texture)
{
    this->setTexture(TextureType::Specular, texture);
} 

void lcf::PhongMaterial::setNormalMap(TextureWrapper texture)
{
    this->setTexture(TextureType::Normal, texture);
}

void lcf::PhongMaterial::setShininess(float shininess)
{
    m_shininess.setValue(shininess);
}
