#pragma once

#include <QObject>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include "Scene.h"
#include <unordered_map>

namespace lcf {
    class SceneManager : public QObject
    {
        Q_OBJECT
    public:
        static SceneManager* instance();
        void initialize(QOpenGLContext *context);
        Scene *get(const QString& name);
        void makeGrassLand();
        void makeTestScene();
        void testShaderToy();
    private:
        SceneManager() = default;
    private:
        QOpenGLContext *m_context = nullptr;
        QOffscreenSurface *m_surface = nullptr;
        std::unordered_map<QString, Scene::UniquePtr> m_scenes;
    };
}