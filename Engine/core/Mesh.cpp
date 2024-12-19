#include "Mesh.h"
#include "utils.h"
#include "ShaderManager.h"


lcf::Mesh::Mesh(const GeometryPtr &geometry) :
    Object3D(),
    m_geometry(geometry),
    m_material_controller(std::make_shared<MaterialController>()), 
    m_skeleton(nullptr),
    m_skeleton_activated(false),
    m_shader_uniform_binder(nullptr),
    m_instance_helper(std::make_shared<InstanceHelper>())
{
}

lcf::Mesh::Mesh(const Mesh &other) :
    Object3D(other),
    m_geometry(other.m_geometry),
    m_material_controller(other.m_material_controller),
    m_skeleton(nullptr),
    m_skeleton_activated(other.m_skeleton_activated),
    m_shader_uniform_binder(other.m_shader_uniform_binder),
    m_instance_helper(other.m_instance_helper ? std::make_shared<InstanceHelper>(*other.m_instance_helper) : std::make_shared<InstanceHelper>())
{
}

void lcf::Mesh::draw()
{
    Object3D::draw();
    if (not m_shader_uniform_binder) { return; }
    if (not m_geometry->isCreated()) { return; }
    m_shader_uniform_binder->bind();
    m_material_controller->bind();
    m_shader_uniform_binder->setUniforms(m_material_controller->asUniformList());
    this->_draw(m_shader_uniform_binder->shader().get());
    m_material_controller->release();
    m_shader_uniform_binder->release();
}

void lcf::Mesh::drawShadow()
{
    if (not m_cast_shadow) { return; }
    Object3D::drawShadow();
    if (not m_geometry->isCreated()) { return; }
    static SharedGLShaderProgramPtr shadow_shader = ShaderManager::instance()->get(ShaderManager::ShadowMap);
    static SharedGLShaderProgramPtr animated_shadow_shader = ShaderManager::instance()->get(ShaderManager::AnimatedShadowMap);
    auto &shader = (m_skeleton and m_skeleton_activated) ? animated_shadow_shader : shadow_shader;
    shader->bind();
    this->_draw(shader.get());
    shader->release();
}

void lcf::Mesh::setSkeleton(SkeletonPtr &&skeleton)
{
    m_skeleton = std::move(skeleton);
}

lcf::Object3D::Type lcf::Mesh::type() const
{
    return Object3D::Type::Mesh;
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

void lcf::Mesh::setShader(const SharedGLShaderProgramPtr &shader)
{
    m_shader_uniform_binder = std::make_shared<ShaderUniformBinder>(shader);
}

void lcf::Mesh::setShaderUniformBinder(const ShaderUniformBinder::SharedPtr &shader_uniform_binder)
{
    m_shader_uniform_binder = shader_uniform_binder;
}

lcf::Mesh::InstanceHelperPtr &lcf::Mesh::instanceHelper()
{
    return m_instance_helper;
}

void lcf::Mesh::setInstanceHelper(const InstanceHelperPtr &instance_helper)
{
    m_instance_helper = instance_helper;
}

const lcf::MaterialController::SharedPtr &lcf::Mesh::materialController() const
{
    return m_material_controller;
}

void lcf::Mesh::setMaterialController(const MaterialController::SharedPtr &material_controller)
{
    m_material_controller = material_controller;
}

void lcf::Mesh::_draw(GLShaderProgram * shader)
{
    shader->setUniformValue("model", this->worldMatrix());
    shader->setUniformValue("normal_matrix", this->normalMatrix());
    if (m_skeleton and m_skeleton_activated) {
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
