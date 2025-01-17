#pragma once

#include "Object3D.h"
#include "Model.h"
#include "Mesh.h"
#include <memory>
#include "TextureWrapper.h"
#include "Light.h"
#include <QTimer>
#include "GLFrameBufferObject.h"
#include "LightArray.h"

namespace lcf {
    class Scene : public Object3D
    {
    public:    
        using SharedPtr = std::shared_ptr<Scene>;
        using UniquePtr = std::unique_ptr<Scene>;
        using ModelList = std::vector<Model::SharedPtr>;
        using MeshList = std::vector<Mesh::SharedPtr>;
        Scene();
        LightArray &lights();
        void addLight(const Light::SharedPtr &light);
        void addModel(const Model::SharedPtr &model);
        void addMesh(const Mesh::SharedPtr &mesh);
        void addSharedChild(const Object3D::SharedPtr &child);
        void draw() override;
        void setSkyboxTexture(TextureWrapper texture);
        QTimer *timer();
        Model *takeModel(int index);
        const ModelList &models() const;
    private:
        void shadowPass();
    private:
        inline static Scene *s_current = nullptr;
    private:
        LightArray m_lights;
        ModelList m_models;
        MeshList m_meshes;
        std::vector<Object3D::SharedPtr> m_shared_children;
        Mesh::SharedPtr m_skybox;
        QTimer m_timer;
        UniqueGLFrameBufferObjectPtr m_fbo;
    };
}