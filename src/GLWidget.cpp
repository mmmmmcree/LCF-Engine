#include "GLWidget.h"
#include <QTimer>
// #include "Camera.h"
#include "GlobalCamera.h"
#include "ShaderToyManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "Scene.h"
#include "TextureManager.h"
#include "Constants.h"
#include "SceneManager.h"

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
    this->initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    ShaderManager::instance()->initialize();
    TextureManager::instance()->initialize(this->context());
    SceneManager::instance()->initialize(this->context());
    ModelManager::instance()->initialize(this->context());
    auto scene = SceneManager::instance()->makeGrassLand();
    Scene::setCurrent(scene);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    ShaderToyManager::instance()->updateActivated();
    Scene::current()->draw();
}

void GLWidget::resizeGL(int w, int h)
{
    w *= this->devicePixelRatio();
    h *= this->devicePixelRatio();
    GlobalCamera::instance()->setViewPort(w, h);
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
