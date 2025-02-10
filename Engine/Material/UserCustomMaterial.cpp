#include "UserCustomMaterial.h"
#include "TextureDispatcher.h"


void lcf::UserCustomMaterial::bind()
{
    // int loc = start_location;
    for (auto &[type, texture] : *m_textures) {
        // texture.bind(loc++);
        const auto &name = this->fromTextureTypeToUniformName(static_cast<TextureType>(type));
        TextureDispatcher::instance()->setTextureByName(name, texture);
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
