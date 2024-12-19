#include "Model.h"

lcf::Model::Model() :
    m_instance_helper(std::make_shared<InstanceHelper>()),
    m_material(Material::newShared())
{
}

lcf::Model *lcf::Model::clone() const
{
    if (not m_created) {
        qDebug() << "lcf::Model::clone() - Model is not created yet!";
        return nullptr;
    }
    Model *cloned_model = new Model;
    cloned_model->m_created = true;
    // cloned_model->m_local = m_local;
    // cloned_model->m_world = m_world;
    // cloned_model->m_world_need_update = m_world_need_update;
    // cloned_model->m_local_decomposed = m_local_decomposed;
    // cloned_model->m_animation_play_state = m_animation_play_state;
    for (auto &mesh : m_meshes) {
        if (not mesh) { continue; }
        auto cloned_mesh = std::make_unique<Mesh>(*mesh);
        cloned_mesh->setParent(cloned_model);
        cloned_model->addMesh(std::move(cloned_mesh));
    }
    cloned_model->m_root_bone = this->processSkeleton(cloned_model->m_bones, nullptr, m_root_bone);
    for (int i = 0; i < cloned_model->m_meshes.size(); ++i) {
        auto &mesh = m_meshes[i];
        if (not mesh->skeleton()) { continue; }
        auto &cloned_mesh = cloned_model->m_meshes[i];
        Skeleton::BonePtrs bones;
        for (auto &bone : mesh->skeleton()->bones()) {
            auto it = cloned_model->m_bones.find(bone->name());
            if (it == cloned_model->m_bones.end()) { continue; }
            bones.emplace_back(it->second);
        }
        cloned_mesh->setSkeleton(std::make_unique<Skeleton>(std::move(bones), mesh->skeleton()->offsetMatrices()));
    }
    for (auto &animation : m_animations) {
        if (not animation) { continue; }
        auto cloned_animation = std::make_unique<Animation>(*animation);
        cloned_animation->updateControlledBones(cloned_model->m_bones);
        cloned_model->addAnimation(std::move(cloned_animation));
    }
    cloned_model->playAnimation();
    return cloned_model;
}

void lcf::Model::draw()
{
    if (m_shader_uniform_binder) {
        m_shader_uniform_binder->bind();
    }
    Object3D::draw();
    if (m_shader_uniform_binder) {
        m_shader_uniform_binder->release();
    }
    m_animation_player.update(1.0f / 60.0f);
}

void lcf::Model::drawShadow()
{
    if (not m_cast_shadow) { return; }
    Object3D::drawShadow();
}

void lcf::Model::create()
{
    if (m_created) { return; }
    m_created = true;
    for (auto &mesh : m_meshes) {
        mesh->geometry()->create();
        mesh->material()->create();
        mesh->setInstanceHelper(m_instance_helper);
    }
}

bool lcf::Model::isCreated() const
{
    return m_created;
}

lcf::Object3D::Type lcf::Model::type() const
{
    return Object3D::Type::Model;
}

bool lcf::Model::hasAnimation() const
{
    return not m_animations.empty();
}

void lcf::Model::setShader(const SharedGLShaderProgramPtr &shader)
{
    m_shader_uniform_binder = std::make_shared<ShaderUniformBinder>(shader);
    for (auto &mesh : m_meshes) {
        mesh->setShaderUniformBinder(m_shader_uniform_binder);
    }
}

void lcf::Model::setShaderUniformBinder(const ShaderUniformBinder::SharedPtr &shader_uniform_binder)
{
    m_shader_uniform_binder = shader_uniform_binder;
    for (auto &mesh : m_meshes) {
        mesh->setShaderUniformBinder(shader_uniform_binder);
    }
}

void lcf::Model::setMaterial(const MaterialPtr &material)
{
    m_material = material;
    for (auto &mesh : m_meshes) {
        mesh->setMaterial(m_material);
    }
}

lcf::Model::InstanceHelperPtr &lcf::Model::instanceHelper()
{
    if (not m_instance_helper) {
        m_instance_helper = std::make_shared<InstanceHelper>();
    }
    return m_instance_helper;
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

void lcf::Model::passSettingsToMeshes()
{
    for (auto &mesh : m_meshes) {
        mesh->setShaderUniformBinder(m_shader_uniform_binder);
        mesh->setInstanceHelper(m_instance_helper);
        mesh->material()->setTextures(m_material->textureInfoMap());
        mesh->setCastShadow(m_cast_shadow);
    }
    this->playAnimation();
}

void lcf::Model::playAnimation(int i, float speed)
{
    m_animation_play_state = {i, speed};
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(true); }
    if (m_animations.empty()) {
        qDebug() << "No animations found!";
        return;
    }
    i = std::clamp(i, 0, static_cast<int>(m_animations.size()) - 1);
    m_animation_player.play(m_animations[i].get(), speed);
}

void lcf::Model::playAnimation()
{
    const auto &[index, speed] = m_animation_play_state;
    if (index == -1) { return; }
    this->playAnimation(index, speed);
}

void lcf::Model::stopAnimation()
{
    m_animation_play_state = {-1, 1.0f};
    m_animation_player.stop();
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(false); }
}

void lcf::Model::addMesh(MeshPtr && mesh)
{
    m_meshes.emplace_back(std::move(mesh));
}

lcf::Model::MeshList & lcf::Model::meshes()
{
    return m_meshes;
}

const lcf::Model::MaterialPtr &lcf::Model::material() const
{
    return m_material;
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
