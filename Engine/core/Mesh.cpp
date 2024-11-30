#include "Mesh.h"


lcf::Mesh::Mesh(const GeometryPtr &geometry, const MaterialPtr &material) :
    Object3D(),
    m_geometry(geometry),
    m_material(material), 
    m_skeleton(nullptr),
    m_skeleton_activated(false),
    m_shader(nullptr)
{
}

lcf::Mesh::Mesh(const Mesh &other) :
    Object3D(other),
    m_geometry(other.m_geometry),
    m_material(other.m_material), 
    m_skeleton(other.m_skeleton ? std::make_unique<Skeleton>(*other.m_skeleton) : nullptr),
    m_skeleton_activated(other.m_skeleton_activated),
    m_shader(other.m_shader)
{
}

void lcf::Mesh::draw()
{
    if (not m_shader or not m_material) { return; }
    m_shader->bind();
    m_shader->setUniformValue("model", this->worldMatrix());
    if (m_skeleton and m_skeleton_activated) {
        m_skeleton->bind(m_shader);
    }
    m_material->bind();
    m_geometry->draw();
    m_material->release();
    m_shader->release();
}

void lcf::Mesh::setSkeleton(SkeletonPtr &&skeleton)
{
    m_skeleton = std::move(skeleton);
}

lcf::Object3D::Type lcf::Mesh::type() const
{
    return Object3D::Type::Mesh;
}

lcf::Geometry *lcf::Mesh::geometry() const
{
    return m_geometry.get();
}

lcf::Material *lcf::Mesh::material() const
{
    return m_material.get();
}

lcf::Skeleton *lcf::Mesh::skeleton() const
{
    return m_skeleton.get();
}

void lcf::Mesh::activateSkeleton(bool active)
{
    m_skeleton_activated = active;
}

void lcf::Mesh::setShader(GLShaderProgram *shader)
{
    m_shader = shader;
}
