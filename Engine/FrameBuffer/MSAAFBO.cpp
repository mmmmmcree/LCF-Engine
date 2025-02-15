#include "MSAAFBO.h"
#include "GLHelper.h"

lcf::MSAAFBO::UniquePtr lcf::MSAAFBO::createUnique(int width, int height, int samples, GLTextureFormat color_format)
{
    return std::make_unique<lcf::MSAAFBO>(width, height, samples, color_format);
}

lcf::MSAAFBO::MSAAFBO(int width, int height, int samples, GLTextureFormat color_format) :
    FrameBufferObject(width, height),
    m_samples(samples),
    m_color_format(color_format)
{
    this->addColorAttachment(NativeTextureWrapper());
    this->update();
}

void lcf::MSAAFBO::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    this->update();
}

void lcf::MSAAFBO::setSamples(int samples)
{
    m_samples = samples;
    this->update();
}

void lcf::MSAAFBO::setColorFormat(GLTextureFormat format)
{
    if (m_color_format == format) { return; }
    m_color_format = format;
    this->update();
}

void lcf::MSAAFBO::update()
{
    this->setColorAttachment(0, GLHelper::generateMSAATexture(m_width, m_height, m_samples, m_color_format));
    this->setDepthStencilAttachment(GLHelper::generateMSAATexture(m_width, m_height, m_samples, GLTextureFormat::D24S8));
}
