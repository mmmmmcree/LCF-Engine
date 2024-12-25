#include "NativeTextureWrapper.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

lcf::NativeTextureWrapper::NativeTextureWrapper()
{
    m_ref_count = new std::atomic<int>(1);
}

lcf::NativeTextureWrapper::~NativeTextureWrapper()
{
    if (--*m_ref_count > 0) { return; }
    delete m_ref_count; m_ref_count = nullptr;
    if (QOpenGLContext::currentContext() and QOpenGLContext::currentContext()->isValid()) {
        auto gl = QOpenGLContext::currentContext()->functions();
        gl->glDeleteTextures(1, &m_texture);
    }
}

lcf::NativeTextureWrapper::NativeTextureWrapper(unsigned int texture) : m_texture(texture)
{
    m_ref_count = new std::atomic<int>(1);
}

lcf::NativeTextureWrapper::NativeTextureWrapper(const NativeTextureWrapper &other) :
    m_texture(other.m_texture),
    m_target(other.m_target),
    m_ref_count(other.m_ref_count)
{
    ++*m_ref_count;
}

lcf::NativeTextureWrapper *lcf::NativeTextureWrapper::operator->()
{
    return this;
}

lcf::NativeTextureWrapper &lcf::NativeTextureWrapper::operator=(const NativeTextureWrapper &other)
{
    if (--*m_ref_count == 0) {
        delete m_ref_count; m_ref_count = nullptr;
        if (QOpenGLContext::currentContext() and QOpenGLContext::currentContext()->isValid()) {
            auto gl = QOpenGLContext::currentContext()->functions();
            gl->glDeleteTextures(1, &m_texture);
        }
    }
    m_texture = other.m_texture;
    m_target = other.m_target;
    m_ref_count = other.m_ref_count;
    ++*m_ref_count;
    return *this;
}

void lcf::NativeTextureWrapper::setTarget(int target)
{
    m_target = target;
}

int lcf::NativeTextureWrapper::target() const
{
    return m_target;
}

void lcf::NativeTextureWrapper::bind(int unit) const
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0 + unit);
    gl->glBindTexture(m_target, m_texture);
}

void lcf::NativeTextureWrapper::release() const
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glBindTexture(m_target, 0);
}

unsigned int lcf::NativeTextureWrapper::id() const
{
    return m_texture;
}
