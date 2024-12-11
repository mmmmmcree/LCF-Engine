#pragma once

#include "GLHelper.h"
#include <variant>
#include "GLTexture.h"
#include "GLShaderProgram.h"
#include "GLFrameBufferObject.h"
#include <vector>

namespace lcf {
    class ShaderToyBuffer
    {
    public:
        using Texture = std::variant<SharedGLTexturePtr, ShaderToyBuffer *>;
        ShaderToyBuffer();
        void setShader(GLShaderProgram *shader);
        GLShaderProgram *shader() const;
        void update();
        void bind(uint unit = 0) const;
        GLuint texture() const;
        void setSize(int width, int height);
        void addTexture(Texture texture);
    private:
        GLShaderProgram *m_shader;
        std::unique_ptr<GLFrameBufferObject> m_read_fbo, m_write_fbo;
        std::vector<Texture> m_textures;
    };
}