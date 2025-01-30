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
#include "Renderer.h"
#include "ControlManager.h"
#include "LImage.h"

using namespace lcf;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMouseTracking(true);
    QTimer *update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, [this] { this->update(); });
    update_timer->start(1000 / 60);
}

void GLWidget::initializeGL()
{
    auto gl = GLFunctions::global();
    gl->initializeOpenGLFunctions();
    gl->glClearColor(0.2f, 0.2f, 0.0f, 1.0f);
    TextureManager::instance()->initialize(this->context());
    SceneManager::instance()->initialize(this->context());
    ModelManager::instance()->initialize(this->context());
    Renderer::instance()->initialize(this->context());
    // SceneManager::instance()->testShaderToy();
    SceneManager::instance()->makeTestScene();
}

void GLWidget::paintGL()
{
    ShaderToyManager::instance()->updateActivated();
    auto current_scene = ControlManager::instance()->currentScene();
    Renderer::instance()->render(current_scene);
}

void GLWidget::resizeGL(int w, int h)
{
    w *= this->devicePixelRatio();
    h *= this->devicePixelRatio();
    GlobalCamera::instance()->setViewport(w, h);
    Renderer::instance()->setRenderSize(w, h);
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
