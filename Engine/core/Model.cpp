#include "Model.h"

lcf::Model *lcf::Model::clone() const
{
    if (not m_created) { return nullptr; }
    Model *model = new Model;
    if (not m_root_bone) {
        for (auto &mesh : m_meshes) {
            if (not mesh) { continue; }
            auto mesh_clone = std::make_unique<Mesh>(*mesh);
            mesh_clone->setParent(model);
            model->addMesh(std::move(mesh_clone));
        }
        return model;
    }
    model->m_root_bone = this->processSkeleton(model->m_bones, nullptr, m_root_bone);
    for (auto &mesh : m_meshes) {
        Skeleton::BonePtrs bones;
        for (auto &bone : mesh->skeleton()->bones()) {
            auto it = model->m_bones.find(bone->name());
            bones.emplace_back(it->second);
        }
        Mesh *mesh_clone = new Mesh(Mesh::GeometryPtr(mesh->geometry()), Mesh::MaterialPtr(mesh->material()));
        mesh_clone->setSkeleton(std::make_unique<Skeleton>(std::move(bones), mesh->skeleton()->offsetMatrices()));
        mesh_clone->setParent(model);
        model->addMesh(MeshPtr(mesh_clone));
    }
    for (auto &animation : m_animations) {
        if (not animation) { continue; }
        auto animation_clone = std::make_unique<Animation>(*animation);
        animation_clone->updateControlledBones(model->m_bones);
        model->addAnimation(std::move(animation_clone));
    }
    return model;
}

void lcf::Model::draw()
{
    Object3D::draw();
    m_animation_player.update(1.0f / 60.0f);
}

void lcf::Model::create()
{
    if (m_created) { return; }
    m_created = true;
    for (auto &mesh : m_meshes) {
        mesh->geometry()->create();
        mesh->material()->create();
    }
}

lcf::Object3D::Type lcf::Model::type() const
{
    return Object3D::Type::Model;
}

bool lcf::Model::hasAnimation() const
{
    return not m_animations.empty();
}

void lcf::Model::setShader(GLShaderProgram *shader)
{
    for (auto &mesh : m_meshes) {
        mesh->setShader(shader);
    }
}

void lcf::Model::setBones(Bone *root_bone, BoneMap &&bone_map)
{
    m_root_bone = root_bone;
    m_bones = std::move(bone_map);
}

void lcf::Model::addAnimation(AnimationPtr &&animation)
{
    m_animations.emplace_back(std::move(animation));
}

void lcf::Model::playAnimation(int i, float speed)
{
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(true); }
    if (m_animations.empty()) {
        qDebug() << "No animations found!";
        return;
    }
    i = std::clamp(i, 0, static_cast<int>(m_animations.size()) - 1);
    m_animation_player.play(m_animations[i].get(), speed);
}

void lcf::Model::stopAnimation()
{
    m_animation_player.stop();
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(false); }
}

void lcf::Model::addMesh(MeshPtr && mesh)
{
    m_meshes.emplace_back(std::move(mesh));
}

lcf::Bone *lcf::Model::processSkeleton(BoneMap &bone_map, Bone *parent, Bone *others_parent) const
{
    Bone *bone = new Bone(*others_parent);
    bone_map.insert(std::make_pair(bone->name(), bone));
    bone->setParent(parent);
    for (auto child : others_parent->children()) {
        this->processSkeleton(bone_map, bone, child);
    }
    return bone;
}
