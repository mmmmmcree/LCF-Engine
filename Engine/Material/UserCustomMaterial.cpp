#include "UserCustomMaterial.h"

void lcf::UserCustomMaterial::setTexture(TextureType texture_type, TextureWrapper texture)
{
    auto iter = m_textures.find(texture_type);
    if (iter == m_textures.end()) {
        m_textures.emplace(std::make_pair(texture_type, texture));
    } else {
        iter->second = texture;
    }
}

void lcf::UserCustomMaterial::bind()
{
    int unit = 0;
    for (auto &[type, texture] : m_textures) {
        texture.bind(unit++);
        qDebug() << "Binding texture of type " << static_cast<int>(type) << " to unit " << unit - 1;
    }
}

lcf::MaterialType lcf::UserCustomMaterial::type() const
{
    return MaterialType::UserCustom;
}
