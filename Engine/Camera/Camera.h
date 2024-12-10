#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include "CameraController.h"
#include "ProjectionProvider.h"

class Camera : public QObject
{
public:
    using ProjectionType = ProjectionProvider::Type;
    using ControllerType = CameraController::Type;
    enum Type {
        TrackballPerspective, 
        FirstPersonPerspective, 
    };
    static Camera *get();
    void initialize();
    const QVector3D &position() const;
    void update();
    void setViewPort(int width, int height);
    void setCameraType(Type type);
    void setCameraController(ControllerType type);
    void setProjectionType(ProjectionType type);
    void processMouseMoveEvent(QMouseEvent *event);
    void processKeyPressEvent(QKeyEvent *event);
    void processKeyReleaseEvent(QKeyEvent *event);
    void processWheelEvent(QWheelEvent *event);
private:
    Camera();
    void setUBO();
private:
public:
    CameraController *m_controller = nullptr;
    ProjectionProvider m_projection_provider;
    QMatrix4x4 m_view;
    GLuint m_ubo = 0;
};