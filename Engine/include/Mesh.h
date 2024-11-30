#pragma once

#include "Geometry.h"
#include "Material.h"
#include "GLShaderProgram.h"
#include "Matrix.h"
#include "Obejct3D.h"
#include "TextureWrapper.h"
#include "Skeleton.h"

namespace lcf {
    class Mesh : public Object3D
    {
    public:
        using GeometryPtr = std::shared_ptr<Geometry>;
        using MaterialPtr = std::shared_ptr<Material>;
        using SkeletonPtr = std::unique_ptr<Skeleton>;
        Mesh(const GeometryPtr &geometry, const MaterialPtr &material);
        Mesh(const Mesh& other);
        void draw() override;
        void setSkeleton(SkeletonPtr &&skeleton);
        Object3D::Type type() const override;
        Geometry* geometry() const;
        Material* material() const;
        Skeleton *skeleton() const;
        void activateSkeleton(bool active);
        void setShader(GLShaderProgram *shader);
    protected:
        GeometryPtr m_geometry;
        MaterialPtr m_material;
        SkeletonPtr m_skeleton;
        bool m_skeleton_activated = false;
        GLShaderProgram *m_shader = nullptr;
    };
}