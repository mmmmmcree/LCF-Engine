#include "FrameBufferObject.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

lcf::FrameBufferObject::FrameBufferObject(int width, int height)
{
    m_fbo = std::make_unique<GLFrameBufferObject>(width, height, GLFrameBufferObject::NoAttachment);
}

bool lcf::FrameBufferObject::bind()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glGetIntegerv(GL_VIEWPORT, m_original_viewport);
    gl->glViewport(0, 0, m_fbo->width(), m_fbo->height());
    return m_fbo->bind();
}

bool lcf::FrameBufferObject::release()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glViewport(m_original_viewport[0], m_original_viewport[1], m_original_viewport[2], m_original_viewport[3]);
    return m_fbo->release();
}

void lcf::FrameBufferObject::addDepthTexture()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    unsigned int texture;
    gl->glGenTextures(1, &texture);
    gl->glBindTexture(GL_TEXTURE_2D, texture);
    gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
        m_fbo->width(), m_fbo->height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    static float border_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    gl->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->handle());
    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
    m_depth_textures.push_back(texture);
}

void lcf::FrameBufferObject::addColorTexture()
{
}

unsigned int lcf::FrameBufferObject::takeDepthTexture(int index)
{
    if (index < 0 or index >= m_depth_textures.size()) { return 0; }
    return m_depth_textures[index];
}

unsigned int lcf::FrameBufferObject::takeColorTexture(int index)
{
    if (index < 0 or index >= m_color_textures.size()) { return 0; }
    return m_color_textures[index];
}
