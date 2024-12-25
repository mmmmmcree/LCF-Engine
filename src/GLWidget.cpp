#include "GLWidget.h"
#include <QTimer>
#include "GlobalCamera.h"
#include "ShaderToyManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "Scene.h"
#include "TextureManager.h"
#include "Constants.h"
#include "SceneManager.h"
#include "GLFunctions.h"
#include "MSAAFBO.h"
#include "RegularFBO.h"
#include "HDRFBO.h"

using namespace lcf;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMouseTracking(true);
    QTimer *update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, [this] { this->update(); });
    update_timer->start(1000 / 60);
}

MSAAFBO::UniquePtr msaa_fbo;  
HDRFBO::UniquePtr before_gamma_fbo;

void GLWidget::initializeGL()
{
    auto gl = GLFunctions::global();
    gl->initializeOpenGLFunctions();
    gl->glClearColor(0.2f, 0.2f, 0.0f, 1.0f);
    ShaderManager::instance()->initialize();
    TextureManager::instance()->initialize(this->context());
    SceneManager::instance()->initialize(this->context());
    ModelManager::instance()->initialize(this->context());
    // auto scene = SceneManager::instance()->makeGrassLand();
    // auto scene = SceneManager::instance()->testPointLightShadow();
    auto scene = SceneManager::instance()->testScene();
    // auto scene = SceneManager::instance()->testDirectionalLightShadow();
    // auto scene = SceneManager::instance()->testShaderToy();
    Scene::setCurrent(scene);
    msaa_fbo = MSAAFBO::createUnique(this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio(), 4, GLTextureFormat::RGBA16F);
    before_gamma_fbo = HDRFBO::createUnique(this->width() * this->devicePixelRatio(), this->height() * this->devicePixelRatio());
}

void GLWidget::paintGL()
{
    auto gl = GLFunctions::global();
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    ShaderToyManager::instance()->updateActivated();
    GlobalCamera::instance()->bind();
    msaa_fbo->bind();
    Scene::current()->draw();
    msaa_fbo->release();
    gl->glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa_fbo->id());
    gl->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, before_gamma_fbo->id());
    gl->glBlitFramebuffer(0, 0, msaa_fbo->width(), msaa_fbo->height(), 0, 0, before_gamma_fbo->width(), before_gamma_fbo->height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    gl->glBindFramebuffer(GL_FRAMEBUFFER, QOpenGLContext::currentContext()->defaultFramebufferObject());
    const auto &shader = ShaderManager::instance()->get(ShaderManager::PostProcess);
    shader->bind();
    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(GL_TEXTURE_2D, before_gamma_fbo->colorAttachment().id());
    Geometry::quad()->draw();
    shader->release();
}

void GLWidget::resizeGL(int w, int h)
{
    w *= this->devicePixelRatio();
    h *= this->devicePixelRatio();
    GlobalCamera::instance()->setViewPort(w, h);
    if (msaa_fbo) {
        msaa_fbo->resize(w, h);
    }
    if (before_gamma_fbo) {
        before_gamma_fbo->resize(w, h);
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    GlobalCamera::instance()->processMouseMoveEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    GlobalCamera::instance()->processWheelEvent(event);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    GlobalCamera::instance()->processKeyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    GlobalCamera::instance()->processKeyReleaseEvent(event);
}
