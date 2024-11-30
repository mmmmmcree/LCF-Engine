#pragma once

#include "../GLHelper.h"
#include <variant>
#include <QOpenGLTexture>
#include <QOpenGLFramebufferObject>
#include <array>

class ShaderToyBuffer
{
public:
    using Texture = std::variant<QOpenGLTexture *, ShaderToyBuffer *>;
    ShaderToyBuffer();
    void setShader(QOpenGLShaderProgram *shader);
    QOpenGLShaderProgram *shader() const;
    void update();
    void bind(uint unit = 0) const;
    GLuint texture() const;
    void setSize(int width, int height);
    void addTexture(Texture texture);
    void addTextures(const QList<Texture> &textures);
private:
    QOpenGLShaderProgram *m_shader;
    std::unique_ptr<QOpenGLFramebufferObject> m_read_fbo, m_write_fbo;
    QList<Texture> m_textures;
};