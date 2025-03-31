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
#include "Constants.h"
#include "GLFunctions.h"
#include "InputProcessor.h"

using namespace lcf;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    this->winId();
    this->setFocusPolicy(Qt::StrongFocus);
    this->setMouseTracking(true);
    QTimer *update_timer = new QTimer(this);
    connect(update_timer, &QTimer::timeout, this, [this] { this->update(); });
    update_timer->start(1000 / 60);
}

void GLWidget::initializeGL()
{
    auto gl = GLFunctions::getGLFunctionsFromCurrentContext();
    gl->initializeOpenGLFunctions();
    gl->glClearColor(0.2f, 0.2f, 0.0f, 1.0f);
    TextureManager::instance()->initialize(this->context());
    SceneManager::instance()->initialize(this->context());
    ModelManager::instance()->initialize(this->context());
    Renderer::instance()->initialize(this->context());
    // SceneManager::instance()->testShaderToy();
    SceneManager::instance()->makeTestScene();
    auto renderer = Renderer::instance();
    auto lut = TextureManager::instance()->loadColorGradingLUT(lcf::path::res_prefix + "Color Grading LUT/color_grading_lut_02.png");
    renderer->setColorGradingLUT(lut);
    // renderer->enableColorGrading(true);
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
    InputProcessor::instance()->processMouseMoveEvent(event);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    InputProcessor::instance()->processWheelEvent(event);
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    InputProcessor::instance()->processKeyEvent(event);
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    InputProcessor::instance()->processKeyEvent(event);
}
