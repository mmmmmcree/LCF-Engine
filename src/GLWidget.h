#pragma once

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
// #include <QOpenGLFunction_4_5_Core>
#include <QOpenGLFunctions_4_5_Core>

class GLWidget : public QOpenGLWidget , protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};