#pragma once

#include "Uniform.h"
#include <memory>
#include <unordered_map>

namespace lcf {
    class ShaderUniformBinder
    {
    public:   
        using SharedPtr = std::shared_ptr<ShaderUniformBinder>;
        using SingleUniformList = std::vector<SingleUniform>;
        using ArrayUniformList = std::vector<ArrayUniform>;
        using NameToIndexMap = std::unordered_map<std::string, int>;
        ShaderUniformBinder(const SharedGLShaderProgramPtr &shader);
        static SharedPtr createShared(const SharedGLShaderProgramPtr &shader);
        void setUniform(const Uniform &uniform);
        void setUniforms(const UniformList &uniforms);
        void setSingleUniform(const SingleUniform &uniform);
        void setSingleUniforms(const SingleUniformList &uniforms);
        void setArrayUniform(const ArrayUniform &uniform);
        void setArrayUniforms(const ArrayUniformList &uniforms);
        const SharedGLShaderProgramPtr& shader() const;
        void bind();
        void release();
    private:
        void setUniformLocation(Uniform &uniform, int location);
    private:
        int m_bound_count = 0;
        SharedGLShaderProgramPtr m_shader;
        UniformList m_uniforms;
        NameToIndexMap m_name_to_index_map;
    };
}