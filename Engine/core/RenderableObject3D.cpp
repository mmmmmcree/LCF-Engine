#include "RenderableObject3D.h"

lcf::RenderableObject3D::RenderableObject3D() :
    Object3D(),
    m_instance_helper(InstanceHelper::createShared()),
    m_material_controller(MaterialController::createShared())
{
}

lcf::RenderableObject3D::RenderableObject3D(const RenderableObject3D &other) :
    Object3D(other),
    m_instance_helper(InstanceHelper::createShared(*other.m_instance_helper)),
    m_material_controller(MaterialController::createShared(*other.m_material_controller)),
    m_cast_shadow(other.m_cast_shadow)
{
}

void lcf::RenderableObject3D::setMaterialType(MaterialType material_type)
{
    m_material_controller->setMaterialType(material_type);
}

lcf::MaterialType lcf::RenderableObject3D::materialType() const
{
    return m_material_controller->materialType();
}

void lcf::RenderableObject3D::setShader(const GLShaderProgram::SharedPtr &shader)
{
    m_material_controller->setShader(shader);
}

lcf::MaterialController *lcf::RenderableObject3D::materialController()
{
    return m_material_controller.get();
}

lcf::InstanceHelper *lcf::RenderableObject3D::instanceHelper()
{
    return m_instance_helper.get();
}

void lcf::RenderableObject3D::setCastShadow(bool cast_shadow)
{
    m_cast_shadow = cast_shadow;
}

bool lcf::RenderableObject3D::isCastShadow() const
{
    return m_cast_shadow;
}

void lcf::RenderableObject3D::setMaterialController(const MaterialController::SharedPtr &material_controller)
{
    if (not material_controller) { return; }
    m_material_controller = material_controller;
}

void lcf::RenderableObject3D::setInstanceHelper(const InstanceHelper::SharedPtr & instance_helper)
{
    if (not instance_helper) { return; }
    m_instance_helper = instance_helper;
}
