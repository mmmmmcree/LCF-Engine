#pragma once

#include "Uniform.h"
#include <memory>

namespace lcf {
    class MMaterial
    {
    public:
        using UniquePtr = std::unique_ptr<MMaterial>;
        using SharedPtr = std::shared_ptr<MMaterial>;
        MMaterial() = default;
        virtual ~MMaterial() = default;
        virtual void bind() = 0;
        void release();
        const UniformList &asUniformList() const;
    protected:
        UniformList m_uniforms;
    };
}