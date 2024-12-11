#pragma once

#include "Geometry.h"
#include "Material.h"
#include "GLShaderProgram.h"
#include "Matrix.h"
#include "Obejct3D.h"
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
        using MaterialPtr = std::shared_ptr<Material>;
        using SkeletonPtr = std::unique_ptr<Skeleton>;
        using InstanceHelperPtr = std::shared_ptr<InstanceHelper>;
        using SharedPtr = std::shared_ptr<Mesh>;
        Mesh(const GeometryPtr &geometry, const MaterialPtr &material);
        Mesh(const Mesh& other);
        void draw() override;
        void setSkeleton(SkeletonPtr &&skeleton);
        void setMaterial(const MaterialPtr &material);
        Object3D::Type type() const override;
        const GeometryPtr &geometry() const;
        const MaterialPtr &material() const;
        const SkeletonPtr &skeleton() const;
        void activateSkeleton(bool active);
        void setShader(const SharedGLShaderProgramPtr &shader);
        void setShaderUniformBinder(const ShaderUniformBinder::SharedPtr &shader_uniform_binder);
        InstanceHelperPtr &instanceHelper();
        void setInstanceHelper(const InstanceHelperPtr &instance_helper);
    protected:
        GeometryPtr m_geometry;
        MaterialPtr m_material;
        SkeletonPtr m_skeleton;
        InstanceHelperPtr m_instance_helper;
        bool m_skeleton_activated = false;
        ShaderUniformBinder::SharedPtr m_shader_uniform_binder;
        // SharedGLShaderProgramPtr m_shader = nullptr;
    };
}