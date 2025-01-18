#include "Model.h"
#include "ControlManager.h"

lcf::Model::Model() :
    m_instance_helper(std::make_shared<InstanceHelper>()),
    m_material_controller(std::make_shared<MaterialController>()),
    m_shader_uniform_binder(ShaderUniformBinder::createShared())
{
}

lcf::Object3DType lcf::Model::type() const
{
    return Object3DType::Model;
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
        mesh->create();
        mesh->setInstanceHelper(m_instance_helper);
    }
    this->playAnimation();
}

bool lcf::Model::isCreated() const
{
    return m_created;
}

void lcf::Model::setShader(const SharedGLShaderProgramPtr &shader)
{
    // m_shader_uniform_binder = ShaderUniformBinder::createShared(shader);
    m_shader_uniform_binder->setShader(shader);
    auto current_scene = ControlManager::instance()->currentScene();
    m_shader_uniform_binder->setUniforms(current_scene->lights().asUniformList());
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

// const lcf::MaterialController::SharedPtr &lcf::Model::materialController() const
// {
//     return m_material_controller;
// }

void lcf::Model::setMaterialType(MaterialType material_type)
{
    m_material_controller->setMaterialType(material_type);
}

lcf::Model::InstanceHelperPtr &lcf::Model::instanceHelper()
{
    if (not m_instance_helper) {
        m_instance_helper = std::make_shared<InstanceHelper>();
    }
    return m_instance_helper;
}

bool lcf::Model::animated() const
{
    return m_animation_player.hasAnimation();
}

void lcf::Model::setBones(Bone *root_bone, BoneMap &&bone_map)
{
    m_root_bone = root_bone;
    m_bones = std::move(bone_map);
}

void lcf::Model::addAnimation(AnimationPtr &&animation)
{
    m_animation_player.playList().emplace_back(std::move(animation)); 
}

void lcf::Model::passSettingsToMeshes()
{
    for (auto &mesh : m_meshes) {
        mesh->setMaterialType(m_material_controller->materialType());
        mesh->setTextures(m_material_controller->textureInfoMap());
        mesh->setShaderUniformBinder(m_shader_uniform_binder);
        mesh->setInstanceHelper(m_instance_helper);
        mesh->setCastShadow(m_cast_shadow);
    }
}

void lcf::Model::playAnimation(int i, float speed)
{
    m_animation_player.play(i, speed);
    if (not this->animated()) { return; }
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(true); }
}

void lcf::Model::playAnimation()
{
    m_animation_player.play();
    if (not this->animated()) { return; }
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(true); }
}

void lcf::Model::stopAnimation()
{
    m_animation_player.stop();
    if (not this->animated()) { return; }
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
