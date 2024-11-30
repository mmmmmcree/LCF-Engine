#pragma once

#include "Mesh.h"
#include "Animation.h"
#include "AnimationPlayer.h"
#include "GLShaderProgram.h"

namespace lcf {
    class Model : public Object3D
    {
    public:    
        using Group = Object3D;
        using MeshPtr = std::unique_ptr<Mesh>;
        using BoneMap = std::unordered_map<std::string, Bone *>;
        using AnimationPtr = std::unique_ptr<Animation>;
        Model() = default;
        Model(const Model &other) = delete;
        Model *clone() const;
        void draw() override;
        void create();
        bool hasAnimation() const;
        void setShader(GLShaderProgram *shader);
        Object3D::Type type() const override;
        void setBones(Bone *root_bone, BoneMap &&bone_map);
        void playAnimation(int i, float speed = 1.0f);
        void stopAnimation();
        void addAnimation(AnimationPtr &&animation);
        void addMesh(MeshPtr &&mesh);
    private:
        Bone *processSkeleton(BoneMap &bone_map, Bone *parent, Bone *others_parent) const;
    private:
        std::vector<MeshPtr> m_meshes;
        std::vector<AnimationPtr> m_animations;
        AnimationPlayer m_animation_player;
        BoneMap m_bones;
        Bone *m_root_bone = nullptr;
        bool m_created = false;
    };
}