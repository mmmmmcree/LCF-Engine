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
    ShaderManager::instance()->initialize();
    Camera::get()->initialize();
    glClearColor(1.0, 1.0, 0.0, 1.0);
    connect(ModelManager::instance(), &ModelManager::initialModelLoaded, this, [this](Model *model) {
        this->makeCurrent();
        model->create();
        this->doneCurrent();
        Scene::current()->addChild(model);
        if (model->hasAnimation()) {
            model->setShader(ShaderManager::instance()->get("animation_debug_3D"));
            model->playAnimation(0, 2.0f);
        } else {
            model->setShader(ShaderManager::instance()->get("texture_debug_3D"));
        }
        // Model *model_copy = model->clone();
        // model_copy->translate(5.0f, 0.0f, 0.0f);
        // model_copy->setScale(2.0f);
        // Scene::current()->addChild(model_copy);
        // model_copy->playAnimation(1, 0.5f);
        // model->playAnimation(0, 2.0f);
        // model_copy->setRotationX(30.0f);
        // model_copy->setScale(2.0f);
    });
}

void GLWidget::paintGL()
{
    ShaderToyManager::updateActivated();
    Camera::get()->update();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    auto scene = Scene::current();
    scene->draw();
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
