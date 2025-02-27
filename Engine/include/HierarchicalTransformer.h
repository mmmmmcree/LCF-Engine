#pragma once

#include "Transformer.h"
#include "Hierarchical.h"

namespace lcf {
    class HierarchicalTransformer : public Transformer, public Hierarchical<HierarchicalTransformer *>
    {
    public:
        using Children = std::vector<HierarchicalTransformer *>;
        HierarchicalTransformer() = default;
        HierarchicalTransformer(const HierarchicalTransformer &other);
        HierarchicalTransformer &operator=(const HierarchicalTransformer &other);
        const Matrix4x4 &getHierarchialMatrix();
        const Matrix4x4 &getInvertedHierarchialMatrix();
        Matrix3x3 getNormalMatrix();
        Vector3D getHierarchialPosition();
        void attachTo(HierarchicalTransformer *parent);
    protected:
        void requireUpdate() override;
    private:
        bool m_inverted_need_update = true;
        Matrix4x4 m_hierarchial_matrix;
        Matrix4x4 m_inverted_hierarchial_matrix;
    };
}