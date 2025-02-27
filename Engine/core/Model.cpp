#include "Model.h"
#include "ShaderManager.h"

lcf::Object3DType lcf::Model::type() const
{
    return Object3DType::Model;
}

void lcf::Model::draw()
{
    m_material_controller->bind();
    Object3D::draw();
    m_material_controller->release();
    m_animation_player.update(1.0f / 60.0f);
}

void lcf::Model::drawShadow(LightType light_type)
{
    if (not this->castShadow()) { return; }
    Object3D::drawShadow(light_type);
}

void lcf::Model::create()
{
    if (m_created) { return; }
    m_created = true;
    for (auto &mesh : m_meshes) {
        mesh->create();
    }
    if (not m_meshes.empty() and this->materialType() == MaterialType::None) {
        this->setMaterialType(m_meshes.front()->materialType());
    }
    this->passSettingsToMeshes();
    this->playAnimation();
}

bool lcf::Model::isCreated() const
{
    return m_created;
}

void lcf::Model::setMaterialType(MaterialType material_type)
{
    auto shader = ShaderManager::instance()->getMaterialShader(material_type, this->animated(), true);
    m_material_controller->setShader(shader);
    m_material_controller->setMaterialType(material_type);
    for (auto &mesh : m_meshes) {
        mesh->setShader(shader);
        mesh->setMaterialType(material_type);
    }
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

void lcf::Model::addAnimation(AnimationPlayer::AnimationPtr &&animation)
{
    m_animation_player.playList().emplace_back(std::move(animation)); 
}

void lcf::Model::passSettingsToMeshes()
{
    for (auto &mesh : m_meshes) {
        mesh->setMaterialType(m_material_controller->materialType());
        mesh->setShader(m_material_controller->shader());
        mesh->setTextures(m_material_controller->textureInfoMap());
        mesh->setCastShadow(this->castShadow());
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
    if (not this->animated()) { return; }
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(true); }
    m_animation_player.play();
}

void lcf::Model::stopAnimation()
{
    m_animation_player.stop();
    if (not this->animated()) { return; }
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(false); }
}

int lcf::Model::currentAnimationIndex() const
{
    return m_animation_player.playingIndex();
}

void lcf::Model::addMesh(MeshPtr && mesh)
{
    mesh->materialController()->setShader(m_material_controller->shader());
    mesh->setInstanceHelper(m_instance_helper);
    m_meshes.emplace_back(std::move(mesh));
}

lcf::Model::MeshList & lcf::Model::meshes()
{
    return m_meshes;
}

const lcf::AnimationPlayer::AnimationList &lcf::Model::animations()
{
    return m_animation_player.playList();
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
