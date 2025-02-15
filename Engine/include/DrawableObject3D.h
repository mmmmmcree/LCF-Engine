#pragma once

#include "Object3D.h"
#include "MaterialController.h"
#include "InstanceHelper.h"
#include "Define.h"

namespace lcf {
    class ModelManager;
    
    class DrawableObject3D : public Object3D
    {
        friend class ModelManager;
    public:
        DrawableObject3D();
        DrawableObject3D(const DrawableObject3D &other);
        virtual void setMaterialType(MaterialType material_type);
        MaterialType materialType() const;
        void setShader(const GLShaderProgram::SharedPtr &shader);
        MaterialController *materialController();
        InstanceHelper *instanceHelper();
    protected:
        void setMaterialController(const MaterialController::SharedPtr &material_controller);
        void setInstanceHelper(const InstanceHelper::SharedPtr &instance_helper);
    protected:
        MaterialController::SharedPtr m_material_controller;
        InstanceHelper::SharedPtr m_instance_helper;
    };
}