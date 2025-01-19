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
    class AssimpLoader;
    class Model;
    class Mesh : public Object3D
    {
        friend class Model;
        friend class AssimpLoader;
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
        void create();
        bool isCreated() const;
        void draw() override;
        void drawShadow(LightType light_type) override;
        void setSkeleton(SkeletonPtr &&skeleton);
        bool animated() const;
        const GeometryPtr &geometry() const;
        const SkeletonPtr &skeleton() const;
        void activateSkeleton(bool active);
        void setShader(const SharedGLShaderProgramPtr &shader);
        void setUniforms(const UniformList &uniforms);
        void setInstanceHelper(const InstanceHelperPtr &instance_helper);
        InstanceHelperPtr &instanceHelper();
        void setMaterialType(MaterialType material_type);
        void setTexture(int texture_type, TextureWrapper texture);
        void setTextures(const MaterialController::TextureInfoMap& texture_info_map);
    private:
        void _draw(GLShaderProgram *shader);
    protected:
        GeometryPtr m_geometry;
        MaterialController::SharedPtr m_material_controller;
        SkeletonPtr m_skeleton;
        InstanceHelperPtr m_instance_helper;
        bool m_skeleton_activated = false;
    };
}