#include "Mesh.h"
#include "utils.h"


lcf::Mesh::Mesh(const GeometryPtr &geometry, const MaterialPtr &material) :
    Object3D(),
    m_geometry(geometry),
    m_material(material), 
    m_skeleton(nullptr),
    m_skeleton_activated(false),
    m_shader(nullptr),
    m_instance_helper(std::make_shared<InstanceHelper>())
{
}

lcf::Mesh::Mesh(const Mesh &other) :
    Object3D(other),
    m_geometry(other.m_geometry),
    m_material(other.m_material), 
    m_skeleton(nullptr),
    m_skeleton_activated(other.m_skeleton_activated),
    m_shader(other.m_shader), 
    m_instance_helper(other.m_instance_helper ? std::make_shared<InstanceHelper>(*other.m_instance_helper) : std::make_shared<InstanceHelper>())
{
}

void lcf::Mesh::draw()
{
    Object3D::draw();
    if (not m_shader or not m_material) { return; }
    if (not m_geometry->isCreated() or not m_material->isCreated()) { return; }
    m_shader->bind();
    m_shader->setUniformValue("model", this->worldMatrix());
    m_shader->setUniformValue("time", utils::elapsed_time_s());
    if (m_skeleton and m_skeleton_activated) {
        const auto &bone_matrices = m_skeleton->boneMatrices();
        if (not bone_matrices.empty()) {
            qDebug() << "bone_matrices size: " << bone_matrices.size();
            m_shader->setUniformValueArray("bone_matrices", bone_matrices.data(), static_cast<int>(bone_matrices.size()));
        }
    }
    m_material->bind();
    int instance_count = m_instance_helper->instanceCount();
    if (instance_count == 0) { m_geometry->draw(); }
    else {
        m_instance_helper->bind();
        m_geometry->drawInstanced(instance_count);
    }
    m_material->release();
    m_shader->release();
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
    m_shader = shader;
}

lcf::Mesh::InstanceHelperPtr &lcf::Mesh::instanceHelper()
{
    return m_instance_helper;
}

void lcf::Mesh::setInstanceHelper(const InstanceHelperPtr &instance_helper)
{
    m_instance_helper = instance_helper;
}

