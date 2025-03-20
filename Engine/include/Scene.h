#pragma once

#include "Object3D.h"
#include "Model.h"
#include "Environment.h"
#include <memory>
#include "TextureWrapper.h"
#include "Light.h"
#include <QTimer>
#include "GLFrameBufferObject.h"
#include "LightArray.h"
#include "PhysicalWorld.h"
#include "Culler.h"

namespace lcf {
    class Scene
    {
    public:    
        using SharedPtr = std::shared_ptr<Scene>;
        using UniquePtr = std::unique_ptr<Scene>;
        using ModelList = std::vector<Model::SharedPtr>;
        using MeshList = std::vector<Mesh::SharedPtr>;
        using GroupList = std::vector<Object3D::SharedPtr>;
        using RenderableList = std::vector<Mesh *>;
        Scene();
        LightArray &lights();
        void addObject3D(const Object3D::SharedPtr &object3d);
        void render();
        void activatePhysicalWorld(bool active);
        Environment *environment();
        QTimer *timer();
        Model *takeModel(int index);
        const ModelList &models() const;
    private:
        void update();
        void shadowPass();
        void addLight(const Light::SharedPtr &light);
        void addModel(const Model::SharedPtr &model);
        void addMesh(const Mesh::SharedPtr &mesh);
        void addGroup(const Object3D::SharedPtr &child);
    private:
        inline static Scene *s_current = nullptr;
    private:
        LightArray m_lights;
        ModelList m_models;
        MeshList m_meshes;
        RenderableList m_renderables;
        GroupList m_groups;
        Environment m_environment;
        QTimer m_timer;
        UniqueGLFrameBufferObjectPtr m_fbo;
        PhysicalWorld::UniquePtr m_physical_world;
        RenderableList m_renderable_list;
        Culler m_culler;
    };
}