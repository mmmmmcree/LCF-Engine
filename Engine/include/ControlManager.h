#pragma once

#include <QObject>
#include "Scene.h"
#include "Model.h"
#include "Mesh.h"
#include "SignalSender.h"

namespace lcf {
    class ControlManager : public QObject
    {
        Q_OBJECT
    public:
        static ControlManager *instance();
        void setCurrentScene(Scene *scene);
        Scene *currentScene() const;
        void setCurrentModel(Model *model);
        Model *currentModel() const;
        void setCurrentMesh(Mesh *mesh);
        Mesh *currentMesh() const;
        void setCurrentObject(Object3D *object);
        Object3D *currentObject() const;
    private:
        ControlManager() = default;
    signals:
        void currentSceneChanged(Scene *scene);
        void currentModelChanged(Model *model);
        void currentMeshChanged(Mesh *mesh);
        void currentObjectChanged(Object3D *object);
    private:
        Scene *m_current_scene = nullptr;
        Model *m_current_model = nullptr;
        Mesh *m_current_mesh = nullptr;
        Object3D *m_current_object = nullptr;
    };
}