#include "HDRFBO.h"
#include "GLHelper.h"

lcf::HDRFBO::UniquePtr lcf::HDRFBO::createUnique(int width, int height)
{
    return std::make_unique<lcf::HDRFBO>(width, height);
}

lcf::HDRFBO::HDRFBO(int width, int height) :
    FrameBufferObject(width, height)
{
    this->addColorAttachment();
    this->update();
}

void lcf::HDRFBO::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    this->update();
}

void lcf::HDRFBO::update()
{
    this->setColorAttachment(0, GLHelper::generateColorTexture(m_width, m_height, GLTextureFormat::RGBA16F));
    this->setDepthStencilAttachment(GLHelper::generateDepthStencilTexture(m_width, m_height));
}
