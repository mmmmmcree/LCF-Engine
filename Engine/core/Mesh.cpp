#include "Mesh.h"
#include "utils.h"


lcf::Mesh::Mesh(const GeometryPtr &geometry, const MaterialPtr &material) :
    Object3D(),
    m_geometry(geometry),
    m_material(material), 
    m_skeleton(nullptr),
    m_skeleton_activated(false),
    m_shader_uniform_binder(nullptr),
    m_instance_helper(std::make_shared<InstanceHelper>())
{
}

lcf::Mesh::Mesh(const Mesh &other) :
    Object3D(other),
    m_geometry(other.m_geometry),
    m_material(other.m_material), 
    m_skeleton(nullptr),
    m_skeleton_activated(other.m_skeleton_activated),
    m_shader_uniform_binder(other.m_shader_uniform_binder),
    m_instance_helper(other.m_instance_helper ? std::make_shared<InstanceHelper>(*other.m_instance_helper) : std::make_shared<InstanceHelper>())
{
}

void lcf::Mesh::draw()
{
    Object3D::draw();
    if (not m_shader_uniform_binder or not m_material) { return; }
    if (not m_geometry->isCreated()) { return; }
    m_shader_uniform_binder->bind();
    const auto &shader = m_shader_uniform_binder->shader();
    shader->setUniformValue("model", this->worldMatrix());
    shader->setUniformValue("normal_matrix", this->normalMatrix());
    if (m_skeleton and m_skeleton_activated) {
        const auto &bone_matrices = m_skeleton->boneMatrices();
        if (not bone_matrices.empty()) {
            shader->setUniformValueArray("bone_matrices", bone_matrices.data(), static_cast<int>(bone_matrices.size()));
        }
    }
    m_material->bind();
    // shader->setUniformValue("material.ambient", m_material->m_ambient);
    // shader->setUniformValue("material.diffuse", m_material->m_diffuse);
    // shader->setUniformValue("material.specular", m_material->m_specular);
    // shader->setUniformValue("material.shininess", m_material->m_shininess);
    // qDebug() << m_material->asUniformList().size();
    m_shader_uniform_binder->setUniforms(m_material->asUniformList());
    int instance_count = m_instance_helper->instanceCount();
    if (instance_count == 0) { m_geometry->draw(); }
    else {
        m_instance_helper->bind();
        m_geometry->drawInstanced(instance_count);
    }
    m_material->release();
    m_shader_uniform_binder->release();
}

void lcf::Mesh::setSkeleton(SkeletonPtr &&skeleton)
{
    m_skeleton = std::move(skeleton);
}

void lcf::Mesh::setMaterial(const MaterialPtr & material)
{
    m_material = material;
}

lcf::Object3D::Type lcf::Mesh::type() const
{
    return Object3D::Type::Mesh;
}

const lcf::Mesh::GeometryPtr &lcf::Mesh::geometry() const
{
    return m_geometry;
}

const lcf::Mesh::MaterialPtr &lcf::Mesh::material() const
{
    return m_material;
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

// void lcf::Mesh::setShader(const SharedGLShaderProgramPtr &shader)
// {
//     m_shader = shader;
// }

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

