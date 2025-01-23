#include "DrawableObject3D.h"

lcf::DrawableObject3D::DrawableObject3D() :
    Object3D(),
    m_instance_helper(InstanceHelper::createShared()),
    m_material_controller(MaterialController::createShared())
{
}

lcf::DrawableObject3D::DrawableObject3D(const DrawableObject3D &other) :
    Object3D(other),
    m_instance_helper(InstanceHelper::createShared(*other.m_instance_helper)),
    m_material_controller(MaterialController::createShared(*other.m_material_controller))
{
}

void lcf::DrawableObject3D::setMaterialType(MaterialType material_type)
{
    m_material_controller->setMaterialType(material_type);
}

lcf::MaterialType lcf::DrawableObject3D::materialType() const
{
    return m_material_controller->materialType();
}

void lcf::DrawableObject3D::setShader(const SharedGLShaderProgramPtr &shader)
{
    m_material_controller->setShader(shader);
}

void lcf::DrawableObject3D::setUniforms(const UniformList &uniforms)
{
    m_material_controller->shaderUniformBinder()->setUniforms(uniforms);
}

lcf::MaterialController *lcf::DrawableObject3D::materialController()
{
    return m_material_controller.get();
}

lcf::InstanceHelper *lcf::DrawableObject3D::instanceHelper()
{
    return m_instance_helper.get();
}

void lcf::DrawableObject3D::setMaterialController(const MaterialController::SharedPtr &material_controller)
{
    if (not material_controller) { return; }
    m_material_controller = material_controller;
}

void lcf::DrawableObject3D::setInstanceHelper(const InstanceHelper::SharedPtr & instance_helper)
{
    if (not instance_helper) { return; }
    m_instance_helper = instance_helper;
}
