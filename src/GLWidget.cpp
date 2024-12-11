#include "GLWidget.h"
#include <QTimer>
#include "Camera/Camera.h"
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
    Camera::instance()->initialize();
    TextureManager::instance()->initialize(this->context());
    SceneManager::instance()->initialize(this->context());
    ModelManager::instance()->initialize(this->context());
    auto scene = SceneManager::instance()->testShaderToy();
    Scene::setCurrent(scene);
}

void GLWidget::paintGL()
{
    Camera::instance()->update();
    ShaderToyManager::instance()->updateActivated();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scene::current()->draw();
    glDisable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h)
{
    w *= this->devicePixelRatio();
    h *= this->devicePixelRatio();
    Camera::instance()->setViewPort(w, h);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    Camera::instance()->processMouseMoveEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    Camera::instance()->processWheelEvent(event);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    Camera::instance()->processKeyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    Camera::instance()->processKeyReleaseEvent(event);
}
