#include "DepthMapFBO.h"
#include "GLHelper.h"

lcf::DepthMapFBO::UniquePtr lcf::DepthMapFBO::createUnique(int width, int height)
{
    return std::make_unique<lcf::DepthMapFBO>(width, height);
}

lcf::DepthMapFBO::DepthMapFBO(int width, int height) : FrameBufferObject(width, height)
{
    this->update();
}

void lcf::DepthMapFBO::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    this->update();
}

void lcf::DepthMapFBO::update()
{
    this->setDepthAttachment(GLHelper::generateDepthMap(m_width, m_height));
}
