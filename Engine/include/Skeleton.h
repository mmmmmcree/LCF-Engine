#pragma once

#include "Matrix.h"
#include "GLShaderProgram.h"
#include "Object3D.h"

namespace lcf {
    class Skeleton
    {
    public:
        using BonePtr = std::unique_ptr<Bone>;
        using BonePtrs = std::vector<BonePtr>;
        using Matrices = std::vector<Matrix4x4>;
        using MatricesPtr = std::shared_ptr<Matrices>;
        Skeleton(BonePtrs &&bones, const MatricesPtr &offset_matrices);
        Skeleton(const Skeleton &other);
        const BonePtrs &bones() const;
        const MatricesPtr &offsetMatrices() const;
        Matrices boneMatrices() const;
    private:
        BonePtrs m_bones;
        MatricesPtr m_offset_matrices;
    };
}