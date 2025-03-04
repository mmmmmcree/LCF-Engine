#include "RegularFBO.h"
#include "GLHelper.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

lcf::RegularFBO::UniquePtr lcf::RegularFBO::createUnique(int width, int height)
{
    return std::make_unique<lcf::RegularFBO>(width, height);
}

lcf::RegularFBO::RegularFBO(int width, int height) : FrameBufferObject(width, height)
{
    this->addColorAttachment();
    this->update();
}

void lcf::RegularFBO::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    this->update();
}

void lcf::RegularFBO::update()
{
    this->setColorAttachment(0, m_color_attachment_generator(m_width, m_height));
    this->setDepthStencilAttachment(GLHelper::generateTexture2DAttachment(m_width, m_height, GLTextureFormat::D24S8));
}
