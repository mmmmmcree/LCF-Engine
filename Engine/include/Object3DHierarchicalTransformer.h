#pragma once

#include "HierarchicalTransformer.h"

namespace lcf {
    class Object3DHierarchicalTransformer : public HierarchicalTransformer
    {
    public:
        using UniquePtr = std::unique_ptr<HierarchicalTransformer>;
        Object3DHierarchicalTransformer() = default;
        Object3DHierarchicalTransformer(const HierarchicalTransformer &other);
        Object3DHierarchicalTransformer &operator=(const HierarchicalTransformer &other);
        const Matrix4x4 &getInvertedHierarchialMatrix() const;
        Matrix3x3 getNormalMatrix() const;
    protected:
        void requireUpdate() override;
    private:
        mutable bool m_inverted_need_update = true;
        mutable Matrix4x4 m_inverted_hierarchial_matrix;
    };
}