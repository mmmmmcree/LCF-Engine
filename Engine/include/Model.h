#pragma once

#include "Mesh.h"
#include "Animation.h"
#include "AnimationPlayer.h"
#include "GLShaderProgram.h"
#include "InstanceHelper.h"
#include "LightArray.h"
#include "RigidBody.h"
#include "PointerWrapper.h"

namespace lcf {
    class ModelManager;
    class AssimpLoader;
    class Model : public RenderableObject3D
    {
        friend class ModelManager;
        friend class AssimpLoader;
    public:
        using SharedPtr = std::shared_ptr<Model>;
        using UniquePtr = std::unique_ptr<Model>;
        using MeshPtr = std::unique_ptr<Mesh>;
        using MeshList = std::vector<MeshPtr>;
        using BoneMap = std::unordered_map<std::string, Bone *>;
        using InstanceHelperPtr = std::shared_ptr<InstanceHelper>;
        using ComponentList = std::vector<PointerWrapper<Component>>;
        Model();
        Object3DType type() const override;
        void update(float dt);
        void draw() override;
        void drawShadow(LightType light_type) override;
        void create();
        bool isCreated() const;
        void setMaterialType(MaterialType material_type) override;
        bool animated() const;
        void playAnimation(int i, float speed = 1.0f);
        void playAnimation();
        void stopAnimation();
        int currentAnimationIndex() const;
        MeshList &meshes();
        const AnimationPlayer::AnimationList &animations();
        template <ComponentConcept C>
        C *getComponent() const;
        template <>
        AnimationPlayer *getComponent() const;
    private:
        void processSkeleton(BoneMap &bone_map, Bone *parent, Bone *others_parent) const;
        void addMesh(MeshPtr &&mesh);
        void setBones(BoneMap &&bone_map);
        void addAnimation(AnimationPlayer::AnimationPtr &&animation);
        Bone * getRootBone() const;
    private:
        MeshList m_meshes;
        BoneMap m_bones;
        bool m_created = false;
        ComponentList m_components;
    private: // 为多线程加载模型准备的，在加载之前设置的状态在加载完毕后可能需要重新设置一遍，故需要保存
        void passSettingsToMeshes();
    };
}

template <lcf::ComponentConcept C>
inline C *lcf::Model::getComponent() const
{
    auto it = std::find_if(m_components.begin() + 1, m_components.end(), [](const PointerWrapper<Component> &p) {
        return p->isTypeConsistent();
    });
    if (it == m_components.end()) { return nullptr; }
    return static_cast<C *>(it->get());
}

template <>
inline lcf::AnimationPlayer *lcf::Model::getComponent() const
{
    return static_cast<AnimationPlayer *>(m_components.front().get());
}