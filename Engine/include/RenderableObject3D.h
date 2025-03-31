#pragma once

#include "Object3D.h"
#include "MaterialController.h"
#include "InstanceHelper.h"
#include "Define.h"

namespace lcf {
    class ModelManager;
    
    class RenderableObject3D : public Object3D
    {
        friend class ModelManager;
    public:
        using SharedPtr = std::shared_ptr<RenderableObject3D>;
        RenderableObject3D();
        RenderableObject3D(const RenderableObject3D &other);
        virtual void setMaterialType(MaterialType material_type);
        MaterialType materialType() const;
        void setShader(const GLShaderProgram::SharedPtr &shader);
        MaterialController *materialController();
        InstanceHelper *instanceHelper();
        virtual void setCastShadow(bool cast_shadow);
        bool isCastShadow() const;
    protected:
        void setMaterialController(const MaterialController::SharedPtr &material_controller);
        void setInstanceHelper(const InstanceHelper::SharedPtr &instance_helper);
    protected:
        MaterialController::SharedPtr m_material_controller;
        InstanceHelper::SharedPtr m_instance_helper;
        bool m_cast_shadow = false;
    };
}