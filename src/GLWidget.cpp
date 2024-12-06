#include "GLWidget.h"
#include <QTimer>
#include <QRandomGenerator>
#include "GLHelper.h"
#include "Camera/Camera.h"
#include "ShaderToys/ShaderToyManager.h"
#include "Constants.h"
#include "Mesh.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "Scene.h"
#include "TextureManager.h"
#include <QRandomGenerator>
#include "InstanceHelper.h"
#include "utils.h"
#include "SceneManager.h"
#include "MModelManager.h"

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
    glClearColor(1.0, 1.0, 0.0, 1.0);
    ShaderManager::instance()->initialize();
    Camera::get()->initialize();
    TextureManager::instance()->initialize(this->context());
    SceneManager::instance()->initialize(this->context());
    MModelManager::instance()->initialize(this->context());
    auto scene = SceneManager::instance()->makeGrassLand();
    Scene::setCurrent(scene);
}

void GLWidget::paintGL()
{
    Camera::get()->update();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scene::current()->draw();
    glDisable(GL_DEPTH_TEST);
}

void GLWidget::resizeGL(int w, int h)
{
    w *= this->devicePixelRatio();
    h *= this->devicePixelRatio();
    Camera::get()->setViewPort(w, h);
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    Camera::get()->processMouseMoveEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    Camera::get()->processWheelEvent(event);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    Camera::get()->processKeyPressEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    Camera::get()->processKeyReleaseEvent(event);
}
