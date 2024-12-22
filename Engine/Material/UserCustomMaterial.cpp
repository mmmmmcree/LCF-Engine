#include "UserCustomMaterial.h"

void lcf::UserCustomMaterial::bind()
{
    int unit = 0;
    for (auto &[type, texture] : *m_textures) {
        texture.bind(unit++);
    }
}

lcf::MaterialType lcf::UserCustomMaterial::type() const
{
    return MaterialType::UserCustom;
}

lcf::UserCustomMaterial::UserCustomMaterial(TextureInfoMap *textures)
{
    m_textures = textures;
}
