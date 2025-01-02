#include "SingleColorAttachmentFBO.h"
#include "GLHelper.h"

lcf::SingleColorAttachmentFBO::SharedPtr lcf::SingleColorAttachmentFBO::createShared(int width, int height, GLTextureFormat color_format)
{
    return std::make_shared<SingleColorAttachmentFBO>(width, height, color_format);
}

lcf::SingleColorAttachmentFBO::SingleColorAttachmentFBO(int width, int height, GLTextureFormat color_format) :
    FrameBufferObject(width, height),
    m_color_format(color_format)
{
    this->addColorAttachment();
    this->update();
}

void lcf::SingleColorAttachmentFBO::resize(int width, int height)
{
    if (m_width == width and m_height == height) { return; }
    m_width = width;
    m_height = height;
    this->update();
}

void lcf::SingleColorAttachmentFBO::setColorFormat(GLTextureFormat color_format)
{
    if (m_color_format == color_format) { return; }
    m_color_format = color_format;
    this->update();
}

void lcf::SingleColorAttachmentFBO::update()
{
    this->setColorAttachment(0, GLHelper::generateColorTexture(m_width, m_height, m_color_format));
}
