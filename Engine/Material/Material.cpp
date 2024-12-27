#include "Material.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>


// void lcf::Material::release()
// {
//     auto gl = QOpenGLContext::currentContext()->functions();
//     // gl->glActiveTexture(GL_TEXTURE0);
//     // gl->glBindTexture(GL_TEXTURE_2D, 0);
//     for (int i = 0; i < 5; ++i) {
//         gl->glActiveTexture(GL_TEXTURE0 + i);
//         gl->glBindTexture(GL_TEXTURE_2D, 0);
//     }
// }

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