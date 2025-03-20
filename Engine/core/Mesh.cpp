#include "Mesh.h"
#include "utils.h"
#include "ShaderManager.h"


lcf::Mesh::Mesh(const GeometryPtr &geometry) :
    RenderableObject3D(),
    m_geometry(geometry),
    m_skeleton(nullptr),
    m_skeleton_activated(false)
{
}

lcf::Mesh::Mesh(const Mesh &other) :
    RenderableObject3D(other),
    m_geometry(other.m_geometry),
    m_skeleton(nullptr),
    m_skeleton_activated(other.m_skeleton_activated)
{
}

lcf::Object3DType lcf::Mesh::type() const
{
    return Object3DType::Mesh;
}

lcf::Mesh::SharedPtr lcf::Mesh::createShared(const GeometryPtr &geometry)
{
    return std::make_shared<Mesh>(geometry);
}

lcf::Mesh::UniquePtr lcf::Mesh::createUnique(const GeometryPtr &geometry)
{
    return std::make_unique<Mesh>(geometry);
}

lcf::Mesh::SharedPtr lcf::Mesh::createShared(const Mesh &other)
{
    return std::make_shared<Mesh>(other);
}

void lcf::Mesh::create()
{
    m_geometry->create();
    m_material_controller->create();
}

bool lcf::Mesh::isCreated() const
{
    return m_geometry->isCreated() and m_material_controller->isCreated();
}

#include "Culler.h"
#include "GlobalCamera.h"

void lcf::Mesh::draw()
{
    if (not m_material_controller->isValid()) { return; }
    if (not m_geometry->isCreated()) { return; }
    m_material_controller->bind();
    this->_draw(m_material_controller->shader().get());
    m_material_controller->release();
}

void lcf::Mesh::drawShadow(LightType light_type)
{
    if (not this->isCastShadow()) { return; }
    if (not m_geometry->isCreated()) { return; }
    const auto &shadow_shader = ShaderManager::instance()->getShadowShader(light_type, this->animated());
    shadow_shader->bindWithTextures();
    this->_draw(shadow_shader.get());
    shadow_shader->release();
}

void lcf::Mesh::setSkeleton(SkeletonPtr &&skeleton)
{
    m_skeleton = std::move(skeleton);
}

bool lcf::Mesh::animated() const
{
    return m_skeleton and m_skeleton_activated;
}

const lcf::Mesh::GeometryPtr &lcf::Mesh::geometry() const
{
    return m_geometry;
}

const lcf::Mesh::SkeletonPtr &lcf::Mesh::skeleton() const
{
    return m_skeleton;
}

void lcf::Mesh::activateSkeleton(bool active)
{
    m_skeleton_activated = active;
}

void lcf::Mesh::setTexture(int texture_type, TextureWrapper texture)
{
    m_material_controller->setTexture(texture_type, texture);
}

void lcf::Mesh::setTextures(const MaterialController::TextureInfoMap & texture_info_map)
{
    m_material_controller->setTextures(texture_info_map);
}

void lcf::Mesh::_draw(GLShaderProgram * shader)
{
    shader->setUniformValue("model", m_transformer.getHierarchialMatrix());
    shader->setUniformValue("normal_matrix", m_transformer.getNormalMatrix());
    if (this->animated()) {
        const auto &bone_matrices = m_skeleton->boneMatrices();
        shader->setUniformValueArray("bone_matrices", bone_matrices.data(), static_cast<int>(bone_matrices.size()));
    }
    int instance_count = m_instance_helper->instanceCount();
    if (instance_count == 0) { m_geometry->draw(); }
    else {
        m_instance_helper->bind();
        m_geometry->drawInstanced(instance_count);
    }
}
