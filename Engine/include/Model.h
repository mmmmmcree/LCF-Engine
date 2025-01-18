#pragma once

#include "Mesh.h"
#include "Animation.h"
#include "AnimationPlayer.h"
#include "GLShaderProgram.h"
#include "InstanceHelper.h"
#include "ShaderUniformBinder.h"

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
        using MeshPtr = std::unique_ptr<Mesh>;
        using MeshList = std::vector<MeshPtr>;
        using BoneMap = std::unordered_map<std::string, Bone *>;
        using AnimationPtr = std::unique_ptr<Animation>;
        using AnimationList = std::vector<AnimationPtr>;
        using InstanceHelperPtr = std::shared_ptr<InstanceHelper>;
        Model();
        Object3DType type() const override;
        void draw() override;
        void drawShadow(LightType light_type) override;
        void create();
        bool isCreated() const;
        void setShader(const SharedGLShaderProgramPtr &shader);
        void setShaderUniformBinder(const ShaderUniformBinder::SharedPtr &shader_uniform_binder);
        const ShaderUniformBinder::SharedPtr &shaderUniformBinder() const;
        const MaterialController::SharedPtr &materialController() const;
        InstanceHelperPtr &instanceHelper();
        bool animated() const;
        void playAnimation(int i, float speed = 1.0f);
        void playAnimation();
        void stopAnimation();
        MeshList &meshes();
    private:
        Bone *processSkeleton(BoneMap &bone_map, Bone *parent, Bone *others_parent) const;
        void addMesh(MeshPtr &&mesh);
        void setBones(Bone *root_bone, BoneMap &&bone_map);
        void addAnimation(AnimationPtr &&animation);
    private:
        MaterialController::SharedPtr m_material_controller;
        ShaderUniformBinder::SharedPtr m_shader_uniform_binder;
        MeshList m_meshes;
        InstanceHelperPtr m_instance_helper;
        AnimationPlayer m_animation_player;
        BoneMap m_bones;
        Bone *m_root_bone = nullptr;
        bool m_created = false;
    private: // 为多线程加载模型准备的，在加载之前设置的状态在加载完毕后可能需要重新设置一遍，故需要保存
        void passSettingsToMeshes();
    };
}