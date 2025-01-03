#include "PhongMaterial.h"
#include "TextureManager.h"

lcf::PhongMaterial::PhongMaterial() : Material()
{
    m_textures.resize(3);
    m_uniforms.emplace_back(SingleUniform("material.diffuse_map", [] { return 0; }));
    m_uniforms.emplace_back(SingleUniform("material.specular_map", 1));
    m_uniforms.emplace_back(SingleUniform("material.normal_map", 2));
    m_uniforms.emplace_back(SingleUniform("material.shininess", [this] { return m_shininess; }));
    m_textures[0] = TextureManager::instance()->get(TextureManager::DefaultDiffuse);
    m_textures[1] = TextureManager::instance()->get(TextureManager::DefaultSpecular);
    m_textures[2] = TextureManager::instance()->get(TextureManager::DefaultNormal);
}

lcf::MaterialType lcf::PhongMaterial::type() const
{
    return MaterialType::Phong;
}

void lcf::PhongMaterial::setDiffuseMap(TextureWrapper texture)
{
    m_textures[0] = texture;
}

void lcf::PhongMaterial::setSpecularMap(TextureWrapper texture)
{
    m_textures[1] = texture;
} 

void lcf::PhongMaterial::setNormalMap(TextureWrapper texture)
{
    m_textures[2] = texture;
}

void lcf::PhongMaterial::setShininess(float shininess)
{
    m_shininess = shininess;
}
