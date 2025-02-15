#pragma once

#include "Uniform.h"
#include <memory>
#include <unordered_map>
#include "TextureWrapper.h"
#include "GLShaderProgram.h"

// namespace lcf {
//     class ShaderUniformBinder
//     {
//     public:   
//         using SharedPtr = std::shared_ptr<ShaderUniformBinder>;
//         using SingleUniformList = std::vector<SingleUniform>;
//         using ArrayUniformList = std::vector<ArrayUniform>;
//         using NameToIndexMap = std::unordered_map<std::string, int>;
//         using SamplerNameList = std::vector<std::string>;
//         using TextureList = std::vector<TextureWrapper>;
//         ShaderUniformBinder() = default;
//         ShaderUniformBinder(const ShaderUniformBinder &other);
//         static SharedPtr createShared();
//         static SharedPtr createShared(const GLShaderProgram::SharedPtr &shader);
//         static SharedPtr createShared(const ShaderUniformBinder &other);
//         void setShader(const GLShaderProgram::SharedPtr &shader);
//         const GLShaderProgram::SharedPtr& shader() const;
//         void bind();
//         void release();
//         bool hasShader() const;
//     private:
//         GLShaderProgram::SharedPtr m_shader = nullptr;
//     };
// }