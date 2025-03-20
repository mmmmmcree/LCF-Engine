#include "Renderer.h"
#include "GlobalCamera.h"
#include "ShaderManager.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "Constants.h"
#include "Define.h"
#include "GLHelper.h"
#include "TextureManager.h"

lcf::Renderer *lcf::Renderer::instance()
{
    static Renderer s_renderer;
    return &s_renderer;
}

void lcf::Renderer::initialize(QOpenGLContext *context)
{
    m_context = context;
    this->updateRenderPassProcedure();
    this->updatePostProcessProcedure();
    m_msaa_fbo = MSAAFBO::createUnique(0, 0, 8, GLTextureFormat::RGB8_UNorm);
    m_post_process_fbo = ScreenFBO::createUnique(0, 0, GLTextureFormat::RGB8_UNorm);
    m_post_process_shader = ShaderManager::instance()->load({
        {GLShader::Vertex, lcf::path::shaders_prefix + "simple2D.vert"}, 
        {GLShader::Fragment, lcf::path::shaders_prefix + "post_process.frag"}, 
    });
    GLHelper::setShaderUniforms(m_post_process_shader.get(), {{"channel0", 0}, {"channel1", 1}});
    m_gamma_correction_enabled.setName("gamma_correction_enabled");
    this->enableGammaCorrection(true);
    m_hdr_enabled.setName("hdr_enabled");
    m_hdr_exposure.setName("exposure");
    m_bloomer = Bloomer::createUnique(2048, 2048);
    this->enableGammaCorrection(true);
    this->enableHDR(true);
    this->enableMSAA(m_msaa_enabled);
    this->enableBloom(m_bloom_enabled);
    m_color_grading_lut_size.setName("color_grading_lut_size");
    m_color_grading_enabled.setName("color_grading_enabled");

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
    m_context->makeCurrent(m_context->surface());
    m_hdr_enabled.setValue(enable);
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
    return m_hdr_enabled.value();
}

void lcf::Renderer::setHDRExposure(float exposure)
{
    m_hdr_exposure.setValue(exposure);
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
    m_gamma_correction_enabled.setValue(enable);
    emit GammaCorrectionEnabledChanged(enable);
}

bool lcf::Renderer::isGammaCorrectionEnabled() const
{
    return m_gamma_correction_enabled.value();
}

void lcf::Renderer::enableColorGrading(bool enable)
{
    m_color_grading_enabled.setValue(enable and m_color_grading_lut.isValid());
}

void lcf::Renderer::setColorGradingLUT(TextureWrapper lut)
{
    m_color_grading_lut_size.setValue(lut.width());
    m_color_grading_lut = lut;
}

void lcf::Renderer::updateRenderPassProcedure()
{
    std::function<void(Scene *)> msaa_render_pass_func = [this](Scene *scene) {
        m_msaa_fbo->bind();
        scene->render();
        m_msaa_fbo->release();
        m_msaa_fbo->blitTo(m_post_process_fbo.get(), FrameBufferObject::ColorAttachment);
    };
    std::function<void(Scene *)> normal_render_pass_func = [this](Scene *scene) {
        m_post_process_fbo->bind();
        scene->render();
        m_post_process_fbo->release();
    };
    m_render_pass_procedure = m_msaa_enabled ? msaa_render_pass_func : normal_render_pass_func;
}

void lcf::Renderer::updatePostProcessProcedure()
{
    std::function<void()> bloom_process_func = [this] {
        m_bloomer->bloom(m_post_process_fbo.get());
        m_post_process_shader->bind();
        m_bloomer->texture().bind(0);
        m_color_grading_lut.bind(1);
        Geometry::quad()->draw();
        m_color_grading_lut.release(1);
        m_bloomer->texture().release(0);
        m_post_process_shader->release();
    };
    std::function<void()> no_bloom_process_func = [this] {
        m_post_process_shader->bind();
        m_post_process_fbo->colorAttachment().bind(0);
        m_color_grading_lut.bind(1);
        Geometry::quad()->draw();
        m_color_grading_lut.release(1);
        m_post_process_fbo->colorAttachment().release(0);
        m_post_process_shader->release();
    };
    m_post_process_procedure = m_bloom_enabled ? bloom_process_func : no_bloom_process_func;
}
