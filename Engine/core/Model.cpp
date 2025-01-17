#include "Model.h"

lcf::Model::Model() :
    m_instance_helper(std::make_shared<InstanceHelper>()),
    m_material_controller(std::make_shared<MaterialController>())
{
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

void lcf::Model::drawShadow(LightType light_type)
{
    if (not m_cast_shadow) { return; }
    Object3D::drawShadow(light_type);
}

void lcf::Model::create()
{
    if (m_created) { return; }
    m_created = true;
    this->passSettingsToMeshes();
    for (auto &mesh : m_meshes) {
        mesh->geometry()->create();
        mesh->materialController()->create();
        mesh->setInstanceHelper(m_instance_helper);
    }
}

bool lcf::Model::isCreated() const
{
    return m_created;
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

const lcf::ShaderUniformBinder::SharedPtr &lcf::Model::shaderUniformBinder() const
{
    return m_shader_uniform_binder;
}

const lcf::MaterialController::SharedPtr &lcf::Model::materialController() const
{
    return m_material_controller;
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
        mesh->materialController()->setMaterialType(m_material_controller->materialType());
        mesh->materialController()->setTextures(m_material_controller->textureInfoMap());
        mesh->setShaderUniformBinder(m_shader_uniform_binder);
        mesh->setInstanceHelper(m_instance_helper);
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
