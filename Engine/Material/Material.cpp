#include "Material.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>


void lcf::Material::release()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(GL_TEXTURE_2D, 0);
}

const lcf::UniformList &lcf::Material::asUniformList() const
{
    return m_uniforms;
}