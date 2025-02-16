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
#include "DrawableObject3D.h"

namespace lcf {
    class AssimpLoader;
    class Model;
    class Mesh : public DrawableObject3D
    {
        friend class AssimpLoader;
        friend class Model;
    public:
        using GeometryPtr = std::shared_ptr<Geometry>;
        using SkeletonPtr = std::unique_ptr<Skeleton>;
        using InstanceHelperPtr = std::shared_ptr<InstanceHelper>;
        using SharedPtr = std::shared_ptr<Mesh>;
        using UniquePtr = std::unique_ptr<Mesh>;
        Mesh(const GeometryPtr &geometry);
        Mesh(const Mesh& other);
        Object3DType type() const override;
        static SharedPtr createShared(const GeometryPtr &geometry);
        static UniquePtr createUnique(const GeometryPtr &geometry);
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
        void setTexture(int texture_type, TextureWrapper texture);
        void setTextures(const MaterialController::TextureInfoMap& texture_info_map);
    private:
        void _draw(GLShaderProgram *shader);
    protected:
        GeometryPtr m_geometry;
        SkeletonPtr m_skeleton;
        bool m_skeleton_activated = false;
    };
}