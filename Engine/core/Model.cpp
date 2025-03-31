#include "Model.h"
#include "ShaderManager.h"
#include "ControlManager.h"
#include "ShapeFactory.h"

lcf::Model::Model() : 
    RenderableObject3D()
{
    m_components.emplace_back(ComponentFactory::createUnique<AnimationPlayer>());
}

lcf::Object3DType lcf::Model::type() const
{
    return Object3DType::Model;
}

void lcf::Model::update(float dt)
{
    for (auto &component : m_components) {
        component->update(dt);
    }
}

void lcf::Model::draw()
{
    m_material_controller->bind();
    for (auto &mesh : m_meshes) {
        mesh->draw();
    }
    m_material_controller->release();
}

void lcf::Model::drawShadow(LightType light_type)
{
    if (not this->isCastShadow()) { return; }
    for (auto &mesh : m_meshes) {
        mesh->drawShadow(light_type);
    }
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
// //todo
    Box box;
    for (const auto &mesh : m_meshes) {
        auto mesh_box = mesh->geometry()->getBoundingShape<Box>();
        if (not mesh_box) { continue; }
        box += *mesh_box;
    }
    auto rigid_body = ComponentFactory::createUnique<RigidBody>();
    rigid_body->setControlledTransformer(&m_transformer);
    rigid_body->setBoxHalfExtents(box.getDimensions() / 2.0f);
    // m_components.emplace_back(std::move(rigid_body));
// //todo
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
    return this->getComponent<AnimationPlayer>()->hasAnimation();
}

void lcf::Model::setBones(BoneMap &&bone_map)
{
    m_bones = std::move(bone_map);
}

void lcf::Model::addAnimation(AnimationPlayer::AnimationPtr &&animation)
{
    this->getComponent<AnimationPlayer>()->playList().emplace_back(std::move(animation));
}

lcf::Bone *lcf::Model::getRootBone() const
{
    if (m_bones.empty()) { return nullptr; }
    return static_cast<Bone *>(m_bones.begin()->second->getRootTransformer());
}

void lcf::Model::passSettingsToMeshes()
{
    for (auto &mesh : m_meshes) {
        mesh->setMaterialType(m_material_controller->materialType());
        mesh->setShader(m_material_controller->shader());
        mesh->setTextures(m_material_controller->textureInfoMap());
        mesh->setCastShadow(this->isCastShadow());
    }
}

void lcf::Model::playAnimation(int i, float speed)
{
    this->getComponent<AnimationPlayer>()->play(i, speed); //- 在模型未create之前调用，则通过AnimationPlayer类记录
    if (not this->animated()) { return; }
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(true); }
}

void lcf::Model::playAnimation()
{
    this->getComponent<AnimationPlayer>()->play();
    if (not this->animated()) { return; }
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(true); }
}

void lcf::Model::stopAnimation()
{
    this->getComponent<AnimationPlayer>()->stop();
    if (not this->animated()) { return; }
    for (auto &mesh : m_meshes) { mesh->activateSkeleton(false); }
}

int lcf::Model::currentAnimationIndex() const
{
    return this->getComponent<AnimationPlayer>()->playingIndex();
}

void lcf::Model::addMesh(MeshPtr && mesh)
{
    mesh->attachTo(this);
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
    return this->getComponent<AnimationPlayer>()->playList();
}

void lcf::Model::setCastShadow(bool cast_shadow)
{
    RenderableObject3D::setCastShadow(cast_shadow);
    for (auto &mesh : m_meshes) {
        mesh->setCastShadow(cast_shadow);
    }
}

void lcf::Model::processSkeleton(BoneMap &bone_map, Bone *parent, Bone *others_parent) const
{
    Bone *bone = new Bone(*others_parent);
    bone_map.insert(std::make_pair(bone->getName(), bone));
    bone->attachTo(parent);
    for (auto child : others_parent->getChildrenTransformer()) {
        this->processSkeleton(bone_map, bone, static_cast<Bone *>(child));
    }
}
