#pragma once

#include "HierarchicalTransformer.h"
#include <string>

namespace lcf {
    class Bone : public HierarchicalTransformer
    {
    public:
        using UniquePtr = std::unique_ptr<Bone>;
        Bone() = default;
        void setName(std::string_view name);
        const std::string &getName() const;
    private:
        std::string m_name;
    };
}
