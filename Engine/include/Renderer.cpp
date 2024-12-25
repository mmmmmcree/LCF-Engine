#include "Renderer.h"
#include "GlobalCamera.h"
#include "ShaderManager.h"

lcf::Renderer *lcf::Renderer::instance()
{
    static Renderer s_renderer;
    return &s_renderer;
}

void lcf::Renderer::setRenderSize(int width, int height)
{
    m_msaa_fbo->resize(width, height);
    m_post_process_fbo->resize(width, height);
}

void lcf::Renderer::render(Scene *scene)
{
    auto gl = GLFunctions::global();
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GlobalCamera::instance()->bind();
    m_msaa_fbo->bind();
    scene->draw();
    m_msaa_fbo->release();
    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msaa_fbo->id());
    gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_post_process_fbo->id());
    gl->glBlitFramebuffer(0, 0, m_msaa_fbo->width(), m_msaa_fbo->height(), 0, 0, m_post_process_fbo->width(), m_post_process_fbo->height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
    m_post_process_fbo->colorAttachment().bind(0);
    m_post_process_shader->bind();
    Geometry::quad()->draw();
    m_post_process_shader->release();
}

lcf::Renderer::Renderer()
{
    m_msaa_fbo = MSAAFBO::createUnique(0, 0, 4, GLTextureFormat::RGBA16F);
    m_post_process_fbo = HDRFBO::createUnique(0, 0);
    m_post_process_shader = ShaderManager::instance()->get(ShaderManager::PostProcess);
}
