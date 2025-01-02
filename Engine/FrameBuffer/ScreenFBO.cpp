#include "ScreenFBO.h"
#include "GLHelper.h"

lcf::ScreenFBO::UniquePtr lcf::ScreenFBO::createUnique(int width, int height, GLTextureFormat color_format)
{
    return std::make_unique<lcf::ScreenFBO>(width, height, color_format);
}

lcf::ScreenFBO::ScreenFBO(int width, int height, GLTextureFormat color_format) :
    FrameBufferObject(width, height),
    m_color_format(color_format)
{
    this->addColorAttachment();
    this->update();
}

void lcf::ScreenFBO::resize(int width, int height)
{
    if (m_width == width and m_height == height) { return; }
    m_width = width;
    m_height = height;
    this->update();
}

void lcf::ScreenFBO::setColorFormat(GLTextureFormat color_format)
{
    if (m_color_format == color_format) { return; }
    m_color_format = color_format;
    this->update();
}

void lcf::ScreenFBO::update()
{
    this->setColorAttachment(0, GLHelper::generateColorTexture(m_width, m_height, m_color_format));
    this->setDepthStencilAttachment(GLHelper::generateDepthStencilTexture(m_width, m_height));
}
