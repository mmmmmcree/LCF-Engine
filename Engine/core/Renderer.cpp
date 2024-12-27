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
    if (m_msaa_enabled) {
        m_msaa_fbo->bind();
        scene->draw();
        m_msaa_fbo->release();
        m_msaa_fbo->blitTo(m_post_process_fbo.get(), FrameBufferObject::ColorAttachment);
    } else {
        m_post_process_fbo->bind();
        scene->draw();
        m_post_process_fbo->release();
    }
    m_post_process_fbo->colorAttachment().bind(0);
    m_post_process_shader_binder->bind();
    Geometry::quad()->draw();
    m_post_process_shader_binder->release();
}

void lcf::Renderer::enableHDR(bool enable)
{
    if (m_hdr_enabled == enable) { return; }
    m_hdr_enabled = enable;
    GLTextureFormat color_format = enable ? GLTextureFormat::RGBA16F : GLTextureFormat::RGBA;
    m_msaa_fbo->setColorFormat(color_format);
    m_post_process_fbo->setColorFormat(color_format);
}

void lcf::Renderer::enableMSAA(bool enable)
{
    if (m_msaa_enabled == enable) { return; }
    m_msaa_enabled = enable;
}

lcf::Renderer::Renderer()
{
    m_msaa_fbo = MSAAFBO::createUnique(0, 0, 4, GLTextureFormat::RGBA);
    m_post_process_fbo = ScreenFBO::createUnique(0, 0, GLTextureFormat::RGBA);
    const auto &post_process_shader = ShaderManager::instance()->get(ShaderManager::PostProcess);
    m_post_process_shader_binder = ShaderUniformBinder::createShared(post_process_shader);
    m_post_process_shader_binder->setSingleUniform({
        "hdr_enabled", [this] { return m_hdr_enabled; }
    });
}
