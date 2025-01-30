#include "Environment.h"
#include "Geometry.h"
#include "ShaderManager.h"
#include "TextureManager.h"

lcf::Environment::Environment()
{
    m_skybox = Mesh::createUnique(Geometry::cube());
    m_skybox->setShader(ShaderManager::instance()->get(ShaderManager::CubicSkybox));
    m_skybox->setMaterialType(MaterialType::UserCustom);
}

void lcf::Environment::setSkyboxTexture(TextureWrapper texture)
{
    if (not texture.isValid()) { return; } 
    //根据纹理目标决定是否需要转为cube map
    if (texture.target() == GLTexture::Target2D) {
        auto shader = ShaderManager::instance()->get(ShaderManager::CubicSkybox);
    }
}
