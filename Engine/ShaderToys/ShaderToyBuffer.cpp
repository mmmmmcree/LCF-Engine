#include "ShaderToyBuffer.h"
#include "Geometry.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>


lcf::ShaderToyBuffer::ShaderToyBuffer() :
    m_shader(nullptr), m_read_fbo(nullptr), m_write_fbo(nullptr)
{
}

void lcf::ShaderToyBuffer::setShader(GLShaderProgram *shader)
{
    m_shader = shader;
}

lcf::GLShaderProgram *lcf::ShaderToyBuffer::shader() const
{
    return m_shader;
}

void lcf::ShaderToyBuffer::update()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    for (int i = 0; i < m_textures.size(); i++) {
        std::visit([&](auto &&arg) {
            arg->bind(i);
        }, m_textures[i]);
    }
    m_write_fbo->bind();
    m_shader->bindWithTextures();
    lcf::Geometry::quad()->draw();
    m_shader->release();
    m_write_fbo->release();
    std::swap(m_read_fbo, m_write_fbo);
    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(GL_TEXTURE_2D, 0);   
}

void lcf::ShaderToyBuffer::bind(uint unit) const
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0 + unit);
    gl->glBindTexture(GL_TEXTURE_2D, m_read_fbo->texture());
}

GLuint lcf::ShaderToyBuffer::texture() const
{
    return m_read_fbo->texture();
}

void lcf::ShaderToyBuffer::setSize(int width, int height)
{
    GLFrameBufferObjectFormat format;
    format.setInternalTextureFormat(GL_RGBA32F);
    format.setAttachment(GLFrameBufferObject::CombinedDepthStencil);
    m_read_fbo = std::make_unique<GLFrameBufferObject>(width, height, format);
    m_write_fbo = std::make_unique<GLFrameBufferObject>(width, height, format);
    auto gl = QOpenGLContext::currentContext()->functions();
    for (GLuint texture : std::vector<GLuint>{m_read_fbo->texture(), m_write_fbo->texture()}) {
        gl->glBindTexture(GL_TEXTURE_2D, texture);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    gl->glBindTexture(GL_TEXTURE_2D, 0);
}

void lcf::ShaderToyBuffer::addTexture(Texture texture)
{
    m_textures.emplace_back(texture);
}
