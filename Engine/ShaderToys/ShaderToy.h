#pragma once

#include "../GLHelper.h"
#include <QOpenGLContext>
#include "ShaderToyBuffer.h"
#include <QList>
#include <QElapsedTimer>
#include <QVector3D>


class ShaderToy
{
public:
    using BufferTexture = std::variant<int, QOpenGLTexture *>; // int for binding buffer as a texture, QOpenGLTexture * for using normal texture
    ShaderToy(int width, int height, const QStringList &frag_paths);
    void setSize(int width, int height);
    void setBuffer(int index, const QList<BufferTexture> &textures = {});
    void update();
    GLuint texture() const;
    void bind(uint unit = 0);
    void release();
private:
    int m_width, m_height;
    std::vector<ShaderToyBuffer> m_buffers;
    std::vector<std::unique_ptr<QOpenGLShaderProgram>> m_shaders;
    GLint m_original_viewport[4];
private:
    QElapsedTimer m_elapsed_timer;
    QVector3D iResolution;
    int iFrame = 0;
};