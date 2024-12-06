#pragma once

#include "Mesh.h"
#include "Animation.h"
#include "AnimationPlayer.h"
#include "GLShaderProgram.h"
#include "InstanceHelper.h"

namespace lcf {
    class ModelManager;
    class AssimpLoader;
    class Model : public Object3D
    {
        friend class ModelManager;
        friend class AssimpLoader;
    public:
        using SharedPtr = std::shared_ptr<Model>;
        using UniquePtr = std::unique_ptr<Model>;
        using Group = Object3D;
        using MeshPtr = std::unique_ptr<Mesh>;
        using MeshList = std::vector<MeshPtr>;
        using BoneMap = std::unordered_map<std::string, Bone *>;
        using AnimationPtr = std::unique_ptr<Animation>;
        using AnimationList = std::vector<AnimationPtr>;
        using InstanceHelperPtr = std::shared_ptr<InstanceHelper>;
        using MaterialPtr = Mesh::MaterialPtr;
        Model();
        Model(const Model &other) = delete;
        Model *clone() const;
        void draw() override;
        void create();
        bool isCreated() const;
        bool hasAnimation() const;
        void setShader(const SharedGLShaderProgramPtr &shader);
        void setMaterial(const MaterialPtr &material);
        InstanceHelperPtr &instanceHelper();
        Object3D::Type type() const override;
        void playAnimation(int i, float speed = 1.0f);
        void playAnimation();
        void stopAnimation();
        MeshList &meshes();
        const MaterialPtr &material() const;
    private:
        Bone *processSkeleton(BoneMap &bone_map, Bone *parent, Bone *others_parent) const;
        void addMesh(MeshPtr &&mesh);
        void setBones(Bone *root_bone, BoneMap &&bone_map);
        void addAnimation(AnimationPtr &&animation);
    private:
        MeshList m_meshes;
        InstanceHelperPtr m_instance_helper;
        AnimationList m_animations;
        AnimationPlayer m_animation_player;
        BoneMap m_bones;
        Bone *m_root_bone = nullptr;
        bool m_created = false;
    private: // 为多线程加载模型准备的，在加载之前设置的状态在加载完毕后可能需要重新设置一遍，故需要保存
        void passSettingsToMeshes();
        using AnimationPlayState = std::pair<int, float>;
        MaterialPtr m_material;        
        SharedGLShaderProgramPtr m_shader = nullptr;
        AnimationPlayState m_animation_play_state = { -1, 1.0f };
    };
}