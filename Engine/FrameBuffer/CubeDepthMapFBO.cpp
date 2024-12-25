#include "CubeDepthMapFBO.h"
#include "GLHelper.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

lcf::CubeDepthMapFBO::UniquePtr lcf::CubeDepthMapFBO::createUnique(int width, int height)
{
    return std::make_unique<CubeDepthMapFBO>(width, height);
}

lcf::CubeDepthMapFBO::CubeDepthMapFBO(int width, int height) : FrameBufferObject(width, height)
{
    this->update();
}

void lcf::CubeDepthMapFBO::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    this->update();
}

void lcf::CubeDepthMapFBO::update()
{
    this->setDepthAttachment(GLHelper::generateCubeDepthMap(m_width, m_height));
}
