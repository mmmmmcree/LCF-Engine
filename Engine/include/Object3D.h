#pragma once

#include "Matrix.h"
#include "GLShaderProgram.h"
#include "Define.h"
#include "SingleUniform.h"
#include "Hierarchical.h"
#include "HierarchicalTransformer.h"
#include "Object3DHierarchicalTransformer.h"
#include <QObject>
#include "Bone.h"

namespace lcf {
    class Object3D
    {
    public:
        using SharedPtr = std::shared_ptr<Object3D>;
        Object3D() = default;
        Object3D(const Object3D &other);
        virtual ~Object3D() = default;
        virtual void draw();
        virtual void drawShadow(LightType light_type);
        virtual Object3DType type() const;
        void attachTo(Object3D* parent);
        void setLocalMatrix(const Matrix4x4 &matrix);
        void translateWorld(float x, float y, float z);
        void translateWorld(const Vector3D &translation);
        void translateLocalXAxis(float x);
        void translateLocalYAxis(float y);
        void translateLocalZAxis(float z);
        void setTranslation(const Vector3D &translation);
        void setRotation(const Quaternion &rotation);
        void setRotation(float angle_deg, float x, float y, float z);
        void setScale(float x, float y, float z);
        void setScale(const Vector3D &scale);
        void rotateLocal(const Quaternion &rotation);
        void rotateLocal(float angle_deg, float x, float y, float z);
        void rotateLocalXAxis(float angle_deg);
        void rotateLocalYAxis(float angle_deg);
        void rotateLocalZAxis(float angle_deg);
        void rotateAround(const Quaternion &rotation, const Vector3D &position);
        void rotateAroundOrigin(const Quaternion &rotation);
        void rotateAroundSelf(const Quaternion &rotation);
        void scale(float x, float y, float z);
        void scale(float factor);
        void scale(const Vector3D &scale);
        Vector3D getLocalPosition() const;
        Vector3D getWorldPosition() const;
        Vector3D translation() const;
        Quaternion rotation() const;
        Vector3D scale() const;
        const Matrix4x4 &getWorldMatrix() const;
        const Matrix4x4  &getInversedWorldMatrix() const;
        Matrix3x3 getNormalMatrix() const;
        virtual void setName(std::string_view name);
        const std::string &getName() const;
        Object3DHierarchicalTransformer &getTransformer();
    protected:
        std::string m_name;
        Object3DHierarchicalTransformer m_transformer;
    };
}
