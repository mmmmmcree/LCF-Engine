#include "DepthCubeMapFBO.h"
#include "GLHelper.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

lcf::DepthCubeMapFBO::UniquePtr lcf::DepthCubeMapFBO::createUnique(int width)
{
    return std::make_unique<DepthCubeMapFBO>(width);
}

lcf::DepthCubeMapFBO::DepthCubeMapFBO(int width) : FrameBufferObject(width, width)
{
    this->update();
}

void lcf::DepthCubeMapFBO::resize(int width)
{
    m_width = width;
    m_height = width;
    this->update();
}

void lcf::DepthCubeMapFBO::update()
{
    this->setDepthAttachment(GLHelper::generateDepthCubeMap(m_width));
}
