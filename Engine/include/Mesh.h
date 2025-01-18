#pragma once

#include "Geometry.h"
#include "MaterialController.h"
#include "GLShaderProgram.h"
#include "Matrix.h"
#include "Object3D.h"
#include "TextureWrapper.h"
#include "Skeleton.h"
#include <QList>
#include "InstanceHelper.h"
#include "ShaderUniformBinder.h"

namespace lcf {
    class Mesh : public Object3D
    {
    public:
        using GeometryPtr = std::shared_ptr<Geometry>;
        using SkeletonPtr = std::unique_ptr<Skeleton>;
        using InstanceHelperPtr = std::shared_ptr<InstanceHelper>;
        using SharedPtr = std::shared_ptr<Mesh>;
        Mesh(const GeometryPtr &geometry);
        Mesh(const Mesh& other);
        Object3DType type() const override;
        static SharedPtr createShared(const GeometryPtr &geometry);
        static SharedPtr createShared(const Mesh& other);
        void draw() override;
        void drawShadow(LightType light_type) override;
        void setSkeleton(SkeletonPtr &&skeleton);
        bool animated() const;
        const GeometryPtr &geometry() const;
        const SkeletonPtr &skeleton() const;
        void activateSkeleton(bool active);
        void setShader(const SharedGLShaderProgramPtr &shader);
        void setShaderUniformBinder(const ShaderUniformBinder::SharedPtr &shader_uniform_binder);
        InstanceHelperPtr &instanceHelper();
        void setInstanceHelper(const InstanceHelperPtr &instance_helper);
        const MaterialController::SharedPtr &materialController() const;
        void setMaterialController(const MaterialController::SharedPtr &material_controller);
    private:
        void _draw(GLShaderProgram *shader);
    protected:
        GeometryPtr m_geometry;
        MaterialController::SharedPtr m_material_controller;
        SkeletonPtr m_skeleton;
        InstanceHelperPtr m_instance_helper;
        bool m_skeleton_activated = false;
        ShaderUniformBinder::SharedPtr m_shader_uniform_binder;
    };
}