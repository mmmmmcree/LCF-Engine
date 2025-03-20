#pragma once

#include "Transformer.h"
#include "Hierarchical.h"
#include <string>

namespace lcf {
    class Bone : public Transformer, public Hierarchical<Bone *>
    {
    public:
        using UniquePtr = std::unique_ptr<Bone>;
        using Children = std::vector<Bone *>;
        Bone() = default;
        Bone(const Bone &other);
        Bone &operator=(const Bone &other);
        const Matrix4x4 &getWorldMatrix();
        void attachTo(Bone *parent);
        const Children &getChildren() const;
        void setName(std::string_view name);
        const std::string &getName() const;
        Bone *getRoot() const;
    protected:
        void requireUpdate(UpdateType type) override;
    private:
        Matrix4x4 m_hierarchial_matrix;
        std::string m_name;
    };
}
