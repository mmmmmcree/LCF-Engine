#pragma once

#include <string>
#include "GLShaderProgram.h"

namespace lcf {
    class  Uniform
    {
    public:
        Uniform() = default;
        virtual ~Uniform();
        Uniform(const Uniform &other);
        virtual void bind(GLShaderProgram *shader) const = 0;
        void setName(std::string_view name);
        const std::string &name() const;
    protected:
        std::string m_name;
    };

}