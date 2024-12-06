#include "ShaderToyBuffer.h"
#include "Geometry.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>

ShaderToyBuffer::ShaderToyBuffer() : m_shader(nullptr), m_read_fbo(nullptr), m_write_fbo(nullptr)
{
}

void ShaderToyBuffer::setShader(QOpenGLShaderProgram *shader)
{
    m_shader = shader;
}

QOpenGLShaderProgram *ShaderToyBuffer::shader() const
{
    return m_shader;
}

void ShaderToyBuffer::update()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    for (int i = 0; i < m_textures.size(); i++) {
        std::visit([&](auto &&arg) {
            arg->bind(i);
        }, m_textures[i]);
    }
    m_write_fbo->bind();
    m_shader->bind();
    lcf::Geometry::quad()->draw();
    m_shader->release();
    m_write_fbo->release();
    std::swap(m_read_fbo, m_write_fbo);
    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(GL_TEXTURE_2D, 0);
}

void ShaderToyBuffer::bind(uint unit) const
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0 + unit);
    gl->glBindTexture(GL_TEXTURE_2D, m_read_fbo->texture());
}

GLuint ShaderToyBuffer::texture() const
{
    return m_read_fbo->texture();
}

void ShaderToyBuffer::setSize(int width, int height)
{
    QOpenGLFramebufferObjectFormat format;
    format.setInternalTextureFormat(GL_RGBA32F);
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    auto tmp = m_read_fbo.release();
    m_read_fbo.reset(new QOpenGLFramebufferObject(width, height, format));
    delete tmp;
    tmp = m_write_fbo.release();
    m_write_fbo.reset(new QOpenGLFramebufferObject(width, height, format));
    delete tmp;
}

void ShaderToyBuffer::addTexture(Texture texture)
{
    m_textures.emplace_back(texture);
}

void ShaderToyBuffer::addTextures(const QList<Texture> &textures)
{
    m_textures.append(textures);
}
