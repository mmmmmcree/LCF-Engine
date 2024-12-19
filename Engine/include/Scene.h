#pragma once

#include "Object3D.h"
#include "Mesh.h"
#include <memory>
#include "TextureWrapper.h"
#include "Light.h"
#include <QTimer>
#include "GLFrameBufferObject.h"

namespace lcf {
    class Scene : public Object3D
    {
    public:    
        using SharedPtr = std::shared_ptr<Scene>;
        using UniquePtr = std::unique_ptr<Scene>;
        using MeshPtr = std::unique_ptr<Mesh>;
        Scene();
        static Scene *global();
        static Scene *current();
        static void setCurrent(Scene *scene);
        void addLight(const Light::SharedPtr &light);
        void addSharedChild(const Object3D::SharedPtr &child);
        void draw() override;
        void setSkyboxTexture(TextureWrapper texture);
        QTimer *timer();
    private:
        void shadowPass();
    private:
        inline static Scene *s_current = nullptr;
    private:
        std::vector<Light::SharedPtr> m_lights;
        std::vector<Object3D::SharedPtr> m_shared_children;
        MeshPtr m_skybox;
        QTimer m_timer;
        UniqueGLFrameBufferObjectPtr m_fbo;
    };
}