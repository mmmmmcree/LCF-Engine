#pragma once

#include "Mesh.h"
#include "Animation.h"
#include "AnimationPlayer.h"
#include "GLShaderProgram.h"
#include "InstanceHelper.h"
#include "LightArray.h"

namespace lcf {
    class ModelManager;
    class AssimpLoader;
    class Model : public DrawableObject3D
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
        Model() = default;
        Object3DType type() const override;
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
    private:
        Bone *processSkeleton(BoneMap &bone_map, Bone *parent, Bone *others_parent) const;
        void addMesh(MeshPtr &&mesh);
        void setBones(Bone *root_bone, BoneMap &&bone_map);
        void addAnimation(AnimationPlayer::AnimationPtr &&animation);
    private:
        AnimationPlayer m_animation_player;
        MeshList m_meshes;
        BoneMap m_bones;
        Bone *m_root_bone = nullptr;
        bool m_created = false;
    private: // 为多线程加载模型准备的，在加载之前设置的状态在加载完毕后可能需要重新设置一遍，故需要保存
        void passSettingsToMeshes();
    };
}