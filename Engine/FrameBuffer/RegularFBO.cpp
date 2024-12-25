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
    this->setColorAttachment(0, GLHelper::generateColorTexture(m_width, m_height, GLTextureFormat::RGBA));
    this->setDepthStencilAttachment(GLHelper::generateDepthStencilTexture(m_width, m_height));
}
