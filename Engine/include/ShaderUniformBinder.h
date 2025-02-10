#pragma once

#include "Uniform.h"
#include <memory>
#include <unordered_map>
#include "TextureWrapper.h"

namespace lcf {
    class ShaderUniformBinder
    {
    public:   
        using SharedPtr = std::shared_ptr<ShaderUniformBinder>;
        using SingleUniformList = std::vector<SingleUniform>;
        using ArrayUniformList = std::vector<ArrayUniform>;
        using NameToIndexMap = std::unordered_map<std::string, int>;
        using SamplerNameList = std::vector<std::string>;
        using TextureList = std::vector<TextureWrapper>;
        ShaderUniformBinder() = default;
        ShaderUniformBinder(const ShaderUniformBinder &other);
        static SharedPtr createShared();
        static SharedPtr createShared(const SharedGLShaderProgramPtr &shader);
        static SharedPtr createShared(const ShaderUniformBinder &other);
        void setShader(const SharedGLShaderProgramPtr &shader);
        void setUniform(const Uniform &uniform);
        void setUniforms(const UniformList &uniforms);
        void setSingleUniform(const SingleUniform &uniform);
        void setSingleUniforms(const SingleUniformList &uniforms);
        void setArrayUniform(const ArrayUniform &uniform);
        void setArrayUniforms(const ArrayUniformList &uniforms);
        const SharedGLShaderProgramPtr& shader() const;
        void bind();
        void release();
        bool hasShader() const;
    private:
        void getSamplerUniformsFromShader();
    private:
        int m_bound_count = 0;
        SharedGLShaderProgramPtr m_shader = nullptr;
        UniformList m_uniforms;
        UniformList m_unset_uniforms;
        NameToIndexMap m_name_to_index_map;
        SamplerNameList m_sampler_names;
        TextureList m_binding_textures;
    };
}