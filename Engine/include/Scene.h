#pragma once

#include "Obejct3D.h"
#include "Mesh.h"
#include <memory>
#include "TextureWrapper.h"
#include <QTimer>

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
        void addSharedChild(const Object3D::SharedPtr &child);
        void draw() override;
        void setSkyboxTexture(TextureWrapper texture);
        QTimer *timer();
    private:
        inline static Scene *s_current = nullptr;
    private:
        std::vector<Object3D::SharedPtr> m_shared_children;
        MeshPtr m_skybox;
        QTimer m_timer;
    };
}