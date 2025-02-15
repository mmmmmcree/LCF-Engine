#include "Bloomer.h"
#include "ShaderManager.h"
#include "Geometry.h"
#include "Constants.h"
#include <cmath>
#include "GLHelper.h"

lcf::Bloomer::UniquePtr lcf::Bloomer::createUnique(int width, int height)
{
    return std::make_unique<Bloomer>(width, height);
}

lcf::Bloomer::Bloomer(int width, int height)
{
    m_screen_fbo = ScreenFBO::createUnique(width, height, GLTextureFormat::RGBA16F);
    int n = static_cast<int>(m_down_sample_fbo_list.size());
    int scale = pow(2, n - 1);
    int w = std::max(s_min_resolution, width / scale);
    int h = std::max(s_min_resolution, height / scale);
    m_down_sample_fbo_list.back() = SingleColorAttachmentFBO::createShared(w, h, GLTextureFormat::RGBA16F);
    for (int i = n - 2; i >= 0; --i) {
        w *= 2; h *= 2;
        m_down_sample_fbo_list[i] = SingleColorAttachmentFBO::createShared(w, h, GLTextureFormat::RGBA16F);
        m_up_sample_fbo_list[i] = SingleColorAttachmentFBO::createShared(w, h, GLTextureFormat::RGBA16F);
    }
    GLShaderProgram::SharedPtr shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "simple2D.vert"},
        {GLShader::Fragment, path::shaders_prefix + "bloom/up_sample.frag"}
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"channel0", 0}, {"channel1", 1}
    });
    // m_up_sample_shader_binder = ShaderUniformBinder::createShared(shader);
    m_up_sample_shader = shader;
    shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "simple2D.vert"},
        {GLShader::Fragment, path::shaders_prefix + "bloom/extract.frag"}
    });
    GLHelper::setShaderUniform(shader.get(), {"channel0", 0});
    // m_extract_shader_binder = ShaderUniformBinder::createShared(shader);
    m_extract_shader = shader;
    shader = ShaderManager::instance()->load({
        {GLShader::Vertex, path::shaders_prefix + "simple2D.vert"},
        {GLShader::Fragment, path::shaders_prefix + "bloom/merge.frag"}
    });
    GLHelper::setShaderUniforms(shader.get(), {
        {"channel0", 0}, {"channel1", 1}
    });
    // m_merge_shader_binder = ShaderUniformBinder::createShared(shader);
    m_merge_shader = shader;
    m_bloom_attenuation.setName("bloom_attenuation");
    m_bloom_radius.setName("bloom_radius");
    m_threshold.setName("threshold");
    m_bloom_intensity.setName("bloom_intensity");
}

void lcf::Bloomer::bloom(FrameBufferObject *source)
{
    m_screen_fbo->resize(source->width(), source->height());
    this->extract(source, m_down_sample_fbo_list.front().get());
    for (int i = 1; i < m_down_sample_fbo_list.size(); ++i) {
        this->downSample(m_down_sample_fbo_list[i - 1].get(), m_down_sample_fbo_list[i].get());
    }
    int n = static_cast<int>(m_up_sample_fbo_list.size());
    this->upSample(m_down_sample_fbo_list[n].get(), m_down_sample_fbo_list[n - 1].get(), m_up_sample_fbo_list.back().get());
    for (int i = n - 1; i >= 1; --i) {
        this->upSample(m_up_sample_fbo_list[i].get(), m_down_sample_fbo_list[i].get(), m_up_sample_fbo_list[i - 1].get());
    }
    this->merge(source, m_up_sample_fbo_list.front().get(), m_screen_fbo.get());
}

const lcf::NativeTextureWrapper & lcf::Bloomer::texture() const
{
    return m_screen_fbo->colorAttachment();
}

void lcf::Bloomer::extract(FrameBufferObject *source, FrameBufferObject *dest)
{
    if (source == dest) { return; }
    dest->bind();
    m_extract_shader->bind();
    source->colorAttachment().bind(0);
    Geometry::quad()->draw();
    source->colorAttachment().release(0);
    m_extract_shader->release();
    dest->release();
}

void lcf::Bloomer::downSample(FrameBufferObject *source, FrameBufferObject *dest)
{
    if (source == dest) { return; }
    source->blitTo(dest, FrameBufferObject::ColorAttachment, GLTextureFilter::Linear);
}

void lcf::Bloomer::upSample(FrameBufferObject *low, FrameBufferObject *high, FrameBufferObject *dest)
{
    if (low == dest or high == dest) { return; }
    dest->bind();
    m_up_sample_shader->bind();
    low->colorAttachment().bind(0);
    high->colorAttachment().bind(1);
    Geometry::quad()->draw();
    high->colorAttachment().release(1);
    low->colorAttachment().release(0);
    m_up_sample_shader->release();
    dest->release();
}

void lcf::Bloomer::merge(FrameBufferObject *source, FrameBufferObject *bloom, FrameBufferObject *dest)
{
    if (source == dest) { return; }
    dest->bind();
    m_merge_shader->bind();
    source->colorAttachment().bind(0);
    bloom->colorAttachment().bind(1);
    Geometry::quad()->draw();
    bloom->colorAttachment().release(1);
    source->colorAttachment().release(0);
    m_merge_shader->release();
    dest->release();
}
