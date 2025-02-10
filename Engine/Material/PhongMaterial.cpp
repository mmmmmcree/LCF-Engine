#include "PhongMaterial.h"
#include "TextureManager.h"

lcf::PhongMaterial::PhongMaterial() : Material()
{
    m_uniforms.emplace_back(SingleUniform("material.shininess", [this] { return m_shininess; }));
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
    m_texture_map["material.diffuse_map"] = texture;
}

void lcf::PhongMaterial::setSpecularMap(TextureWrapper texture)
{
    m_texture_map["material.specular_map"] = texture;
} 

void lcf::PhongMaterial::setNormalMap(TextureWrapper texture)
{
    m_texture_map["material.normal_map"] = texture;
}

void lcf::PhongMaterial::setShininess(float shininess)
{
    m_shininess = shininess;
}
