#pragma once

#include "MaterialController.h"
#include "ShaderUniformBinder.h"

namespace lcf {
    class ShaderContextBinder
    {
    public:
        using SingleUniformList = std::vector<SingleUniform>;
        using ArrayUniformList = std::vector<ArrayUniform>;
        ShaderContextBinder();
        ShaderContextBinder(const ShaderContextBinder &other);
        void bind();
        void release();
        void setShader(const SharedGLShaderProgramPtr &shader);
        void setUniform(const Uniform &uniform);
        void setUniforms(const UniformList &uniforms);
        void setSingleUniform(const SingleUniform &uniform);
        void setSingleUniforms(const SingleUniformList &uniforms);
        void setArrayUniform(const ArrayUniform &uniform);
        void setArrayUniforms(const ArrayUniformList &uniforms);
        const SharedGLShaderProgramPtr& shader() const;
        bool isValid() const;
    private:
        MaterialController::SharedPtr m_material_controller;
        ShaderUniformBinder::SharedPtr m_shader_uniform_binder;
    };
}