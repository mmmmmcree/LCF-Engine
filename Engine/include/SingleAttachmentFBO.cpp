#include "SingleAttachmentFBO.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "GLHelper.h"

lcf::SingleAttachmentFBO::UniquePtr lcf::SingleAttachmentFBO::createUnique(int width, int height, AttachmentType attachment_type)
{
    return std::make_unique<SingleAttachmentFBO>(width, height, attachment_type);
}

lcf::SingleAttachmentFBO::SingleAttachmentFBO(int width, int height, AttachmentType attachment_type)
{
    m_width = width;
    m_height = height;
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glGenFramebuffers(1, &m_fbo);
    switch (attachment_type) {
        case DepthMap : { this->configureDepthMapAttachment(); } break;
        case CubeDepthMap : { this->configureCubeDepthMapAttachment(); } break;
    }
}

lcf::SingleAttachmentFBO::~SingleAttachmentFBO()
{
    if (QOpenGLContext::currentContext() and QOpenGLContext::currentContext()->isValid()) {
        auto gl = QOpenGLContext::currentContext()->functions();
        gl->glDeleteFramebuffers(1, &m_fbo);
    }
}

void lcf::SingleAttachmentFBO::bind()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glGetIntegerv(GL_VIEWPORT, m_original_viewport);
    gl->glViewport(0, 0, m_width, m_height);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void lcf::SingleAttachmentFBO::release()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glViewport(m_original_viewport[0], m_original_viewport[1], m_original_viewport[2], m_original_viewport[3]);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
}

int lcf::SingleAttachmentFBO::width() const
{
    return m_width;
}

int lcf::SingleAttachmentFBO::height() const
{
    return m_height;
}

void lcf::SingleAttachmentFBO::configureDepthMapAttachment()
{
    m_texture = GLHelper::generateDepthMap(m_width, m_height);
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture.texture(), 0);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
}

void lcf::SingleAttachmentFBO::configureCubeDepthMapAttachment()
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    m_texture = GLHelper::generateCubeDepthMap(m_width, m_height);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_texture.texture(), 0);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
}

const lcf::NativeTextureWrapper & lcf::SingleAttachmentFBO::texture() const
{
    return m_texture;
}
