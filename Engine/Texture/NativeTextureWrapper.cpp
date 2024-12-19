#include "NativeTextureWrapper.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

lcf::NativeTextureWrapper::NativeTextureWrapper(unsigned int texture) :
    m_texture(texture)
{
}

lcf::NativeTextureWrapper *lcf::NativeTextureWrapper::operator->()
{
    return this;
}

void lcf::NativeTextureWrapper::bind(int unit) const
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0 + unit);
    gl->glBindTexture(GL_TEXTURE_2D, m_texture);
}

void lcf::NativeTextureWrapper::release() const
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glBindTexture(GL_TEXTURE_2D, 0);
}
