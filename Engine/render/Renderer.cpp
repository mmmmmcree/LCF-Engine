#include "Renderer.h"
#include "GlobalCamera.h"
#include "ShaderManager.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "Constants.h"
#include "GLHelper.h"

lcf::Renderer *lcf::Renderer::instance()
{
    static Renderer s_renderer;
    return &s_renderer;
}

void lcf::Renderer::initialize(QOpenGLContext *context)
{
    m_context = context;
    m_surface = new QOffscreenSurface(nullptr, this);
    m_surface->setFormat(context->format());
    m_surface->create();
    this->updateRenderPassProcedure();
    this->updatePostProcessProcedure();
    m_msaa_fbo = MSAAFBO::createUnique(0, 0, 8, GLTextureFormat::RGBA8U);
    m_post_process_fbo = ScreenFBO::createUnique(0, 0, GLTextureFormat::RGBA8U);
    SharedGLShaderProgramPtr post_process_shader = ShaderManager::instance()->load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple2D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "post_process.frag"}, 
    });
    GLHelper::setShaderUniform(post_process_shader.get(), {"channel0", 0});
    m_post_process_shader_binder = ShaderUniformBinder::createShared(post_process_shader);
    m_post_process_shader_binder->setUniforms({
        SingleUniform("hdr_enabled", [this] { return m_hdr_enabled; }),
        SingleUniform("gamma_correction_enabled", [this] { return m_gamma_correction_enabled; }),
        SingleUniform("exposure", [this] { return m_hdr_exposure; })
    });
    m_bloomer = Bloomer::createUnique(2048, 2048);
    this->enableGammaCorrection(m_gamma_correction_enabled);
    this->enableHDR(m_hdr_enabled);
    this->enableMSAA(m_msaa_enabled);
    this->enableBloom(m_bloom_enabled);
}

void lcf::Renderer::setRenderSize(int width, int height)
{
    m_msaa_fbo->resize(width, height);
    m_post_process_fbo->resize(width, height);
}

void lcf::Renderer::render(Scene *scene)
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GlobalCamera::instance()->bind();
    m_render_pass_procedure(scene);
    m_post_process_procedure();
}

void lcf::Renderer::enableHDR(bool enable)
{
    m_context->makeCurrent(m_surface);
    m_hdr_enabled = enable;
    GLTextureFormat color_format = enable ? GLTextureFormat::RGBA16F : GLTextureFormat::RGBA8_UNorm;
    m_msaa_fbo->setColorFormat(color_format);
    m_post_process_fbo->setColorFormat(color_format);
    if (not enable) {
        this->enableBloom(false);
    }
    emit HDREnabledChanged(enable);
}

bool lcf::Renderer::isHDREnabled() const
{
    return m_hdr_enabled;
}

void lcf::Renderer::setHDRExposure(float exposure)
{
    m_hdr_exposure = exposure;
}

void lcf::Renderer::enableMSAA(bool enable)
{
    m_msaa_enabled = enable;
    this->updateRenderPassProcedure();
    emit MSAAEnabledChanged(enable);
}

bool lcf::Renderer::isMSAAEnabled() const
{
    return m_msaa_enabled;
}

void lcf::Renderer::enableBloom(bool enable)
{
    m_bloom_enabled = enable;
    this->updatePostProcessProcedure();
    emit BloomEnabledChanged(enable);
}

bool lcf::Renderer::isBloomEnabled() const
{
    return m_bloom_enabled;
}

void lcf::Renderer::enableGammaCorrection(bool enable)
{
    m_gamma_correction_enabled = enable;
    emit GammaCorrectionEnabledChanged(enable);
}

bool lcf::Renderer::isGammaCorrectionEnabled() const
{
    return m_gamma_correction_enabled;
}

void lcf::Renderer::updateRenderPassProcedure()
{
    std::function<void(Scene *)> msaa_render_pass_func = [this](Scene *scene) {
        m_msaa_fbo->bind();
        scene->draw();
        m_msaa_fbo->release();
        m_msaa_fbo->blitTo(m_post_process_fbo.get(), FrameBufferObject::ColorAttachment);
    };
    std::function<void(Scene *)> normal_render_pass_func = [this](Scene *scene) {
        m_post_process_fbo->bind();
        scene->draw();
        m_post_process_fbo->release();
    };
    m_render_pass_procedure = m_msaa_enabled ? msaa_render_pass_func : normal_render_pass_func;
}

void lcf::Renderer::updatePostProcessProcedure()
{
    std::function<void()> bloom_process_func = [this] {
        m_bloomer->bloom(m_post_process_fbo.get());
        m_bloomer->texture().bind(0);
        m_post_process_shader_binder->bind();
        Geometry::quad()->draw();
        m_post_process_shader_binder->release();
        m_bloomer->texture().release(0);
    };
    std::function<void()> no_bloom_process_func = [this] {
        m_post_process_fbo->colorAttachment().bind(0);
        m_post_process_shader_binder->bind();
        Geometry::quad()->draw();
        m_post_process_shader_binder->release();
        m_post_process_fbo->colorAttachment().release(0);
    };
    m_post_process_procedure = m_bloom_enabled ? bloom_process_func : no_bloom_process_func;
}
