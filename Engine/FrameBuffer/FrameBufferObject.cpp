#include "FrameBufferObject.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include "GLHelper.h"
#include "GLFunctions.h"

lcf::FrameBufferObject::UniquePtr lcf::FrameBufferObject::createUnique(int width, int height)
{
    return std::make_unique<FrameBufferObject>(width, height);
}

lcf::FrameBufferObject::FrameBufferObject(int width, int height)
{
    m_width = width;
    m_height = height;
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glGenFramebuffers(1, &m_fbo);
}

lcf::FrameBufferObject::~FrameBufferObject()
{
    if (QOpenGLContext::currentContext() and QOpenGLContext::currentContext()->isValid()) {
        auto gl = QOpenGLContext::currentContext()->functions();
        gl->glDeleteFramebuffers(1, &m_fbo);
    }
    m_color_attachments.clear();
}

void lcf::FrameBufferObject::checkStatus()
{
    this->bind();
    auto gl = QOpenGLContext::currentContext()->functions();
    auto status = gl->glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
        case GL_FRAMEBUFFER_COMPLETE: {
            qDebug() << "FrameBufferObject::checkStatus() complete";
        } break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: {
            qDebug() << "FrameBufferObject::checkStatus() incomplete attachment";
        } break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: {
            qDebug() << "FrameBufferObject::checkStatus() incomplete missing attachment";
        } break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER : {
            qDebug() << "FrameBufferObject::checkStatus() incomplete draw buffer";
        } break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER : {
            qDebug() << "FrameBufferObject::checkStatus() incomplete read buffer";
        } break;
        case GL_FRAMEBUFFER_UNSUPPORTED: {
            qDebug() << "FrameBufferObject::checkStatus() unsupported";
        } break;
        default: {
            qDebug() << "FrameBufferObject::checkStatus() unknown";
        } break;
    }
    this->release();
}

void lcf::FrameBufferObject::bind()
{
    this->_bind();
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glViewport(0, 0, m_width >> m_mip_map_level, m_height >> m_mip_map_level);
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void lcf::FrameBufferObject::blitTo(FrameBufferObject *target, AttachmentType attachment_types, GLTextureFilter color_filter)
{
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    int prev_bound_fbo;
    gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev_bound_fbo);
    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id());
    gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->id());
    if (attachment_types & ColorAttachment) {
        gl->glBlitFramebuffer(0, 0, this->width(), this->height(), 0, 0, target->width(), target->height(), GL_COLOR_BUFFER_BIT, color_filter);
    }
    if (attachment_types & DepthAttachment) {
        gl->glBlitFramebuffer(0, 0, this->width(), this->height(), 0, 0, target->width(), target->height(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }
    if (attachment_types & DepthStencilAttachment) {
        gl->glBlitFramebuffer(0, 0, this->width(), this->height(), 0, 0, target->width(), target->height(), GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
    gl->glBindFramebuffer(GL_FRAMEBUFFER, prev_bound_fbo);
}

void lcf::FrameBufferObject::release()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    auto &viewport = s_viewport_stack.top();
    int fbo = s_fbo_stack.top();
    gl->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    gl->glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    s_viewport_stack.pop();
    s_fbo_stack.pop();
}

int lcf::FrameBufferObject::width() const
{
    return m_width;
}

int lcf::FrameBufferObject::height() const
{
    return m_height;
}

unsigned int lcf::FrameBufferObject::id() const
{
    return m_fbo;
}

void lcf::FrameBufferObject::addColorAttachment()
{
    this->addColorAttachment(GLHelper::generateTexture2DAttachment(m_width, m_height, GLTextureFormat::RGBA32U));
}

void lcf::FrameBufferObject::addColorAttachment(const NativeTextureWrapper &color_attachment)
{
    this->_bind();
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + static_cast<int>(m_color_attachments.size()), color_attachment.textureId(), m_mip_map_level);
    this->_release();
    m_color_attachments.push_back(color_attachment);
}

void lcf::FrameBufferObject::setColorAttachment(int index, const NativeTextureWrapper &color_attachment)
{
    if (index < 0 or index >= static_cast<int>(m_color_attachments.size())) { return; }
    m_color_attachments[index] = color_attachment;
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    this->_bind();
    gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, color_attachment.textureId(), m_mip_map_level);
    this->_release();
}

void lcf::FrameBufferObject::setDepthAttachment(const NativeTextureWrapper &depth_attachment)
{
    this->_bind();
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_attachment.textureId(), m_mip_map_level);
    this->_release();
    m_depth_attachment = depth_attachment;
}

void lcf::FrameBufferObject::setDepthStencilAttachment(const NativeTextureWrapper &depth_stencil_attachment)
{
    this->_bind();
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_stencil_attachment.textureId(), m_mip_map_level);
    this->_release();
    m_depth_stencil_attachment = depth_stencil_attachment;
}

const lcf::NativeTextureWrapper &lcf::FrameBufferObject::colorAttachment(int index) const
{
    return m_color_attachments.at(index);
}

const lcf::NativeTextureWrapper &lcf::FrameBufferObject::depthAttachment() const
{
    return m_depth_attachment;
}

const lcf::NativeTextureWrapper &lcf::FrameBufferObject::depthStencilAttachment() const
{
    return m_depth_stencil_attachment;
}

void lcf::FrameBufferObject::setMipMapLevel(int level)
{
    this->_bind();
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    gl->glViewport(0, 0, m_width >> m_mip_map_level, m_height >> m_mip_map_level);
    for (int i = 0; i < m_color_attachments.size(); ++i) {
        gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, m_color_attachments[i].textureId(), level);
    }
    gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_attachment.textureId(), level);
    gl->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, m_depth_stencil_attachment.textureId(), level);
    this->_release();
    m_mip_map_level = level;
}

void lcf::FrameBufferObject::_bind()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    int prev_fbo;
    Viewport prev_viewport;
    gl->glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prev_fbo);
    gl->glGetIntegerv(GL_VIEWPORT, prev_viewport.data());
    s_fbo_stack.push(prev_fbo);
    s_viewport_stack.push(prev_viewport);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void lcf::FrameBufferObject::_release()
{
    this->release();
}
