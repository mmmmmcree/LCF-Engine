#include "Material.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>


void lcf::Material::bind()
{
    for (int i = 0; i < m_textures.size(); ++i) {
        m_textures[i].bind(i);
    }
}

void lcf::Material::release()
{
    for (int i = 0; i < m_textures.size(); ++i) {
        m_textures[i].release(i);
    }
}

const lcf::UniformList &lcf::Material::asUniformList() const
{
    return m_uniforms;
}