#pragma once

#include "Transformer.h"
#include "Hierarchical.h"

namespace lcf {
    class HierarchicalTransformer : public Transformer, public Hierarchical<HierarchicalTransformer *>
    {
    public:
        using UniquePtr = std::unique_ptr<HierarchicalTransformer>;
        using Parent = Hierarchical *;
        using Children = std::vector<HierarchicalTransformer *>;
        HierarchicalTransformer() = default;
        HierarchicalTransformer(const HierarchicalTransformer &other);
        HierarchicalTransformer &operator=(const HierarchicalTransformer &other);
        const Matrix4x4 &getWorldMatrix() const;
        Vector3D getWorldPosition() const;
        Vector3D getWorldScale() const;
        void attachTo(HierarchicalTransformer *parent);
        const Children &getChildrenTransformer() const;
        Parent getRootTransformer() const;
    protected:
        void requireUpdate() override;
    private:
        mutable Matrix4x4 m_hierarchial_matrix;
    };
}
