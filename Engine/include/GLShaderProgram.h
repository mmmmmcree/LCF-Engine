#pragma once

#include <QOpenGLShaderProgram>
#include <memory>
#include "TextureWrapper.h"
#include <QSet>

namespace lcf {
    using GLShader = QOpenGLShader;

    class Uniform;

    class GLShaderProgram : public QOpenGLShaderProgram
    {
    public:
        using UniformPtrList = std::vector<const Uniform *>;
        using SharedPtr = std::shared_ptr<GLShaderProgram>;
        using UniquePtr = std::unique_ptr<GLShaderProgram>;
        using UniformNameSet = QSet<std::string>;
        using SamplerNameList = std::vector<std::string>;
        using TextureList = std::vector<TextureWrapper>;
        GLShaderProgram(QObject *parent = nullptr);
        bool link() override;
        bool bindWithTextures(); //- 根据sampler uniform的name从TextureDispatcher中获得纹理并分配纹理单元
        bool bind();
        void release();
    private:
        void assignDefaultValueToUniform(const char *uniform_name, unsigned int uniform_type);
        void onUniformUpdated(const Uniform *uniform);
    private:
        UniformNameSet m_uniform_names;
        UniformPtrList m_unset_uniforms;
        SamplerNameList m_sampler_names;
        TextureList m_binding_textures;
    };
}