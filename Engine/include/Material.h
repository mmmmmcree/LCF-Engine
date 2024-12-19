#pragma once

#include "Uniform.h"
#include <memory>
#include "Define.h"

namespace lcf {
    class Material
    {
    public:
        using UniquePtr = std::unique_ptr<Material>;
        using SharedPtr = std::shared_ptr<Material>;
        Material() = default;
        virtual ~Material() = default;
        virtual void bind() = 0;
        virtual MaterialType type() const = 0;
        void release();
        const UniformList &asUniformList() const;
    protected:
        UniformList m_uniforms;
    };
}