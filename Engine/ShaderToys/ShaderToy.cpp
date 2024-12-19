#include "ShaderToy.h"
#include <QOffscreenSurface>
#include <QOpenGLFunctions>
#include "ShaderManager.h"
#include "Constants.h"
#include "utils.h"

lcf::ShaderToy::ShaderToy(int width, int height, const QStringList &frag_paths) :
    m_buffers(frag_paths.size())
{
    this->setSize(width, height);
    for (int i = 0; i < frag_paths.size(); ++i) {
        m_shaders.emplace_back(
            ShaderManager::instance()->load({
                {QOpenGLShader::Vertex, path::shaders_prefix + "simple2D.vert"}, 
                {QOpenGLShader::Fragment, frag_paths[i]}
            })
        );
        auto shader = m_shaders.back().get(); 
        GLHelper::setShaderUniforms(shader, {
            {"iResolution", iResolution},
            {"iChannel0", 0}, {"iChannel1", 1}, {"iChannel2", 2}, {"iChannel3", 3},
        });
        m_buffers[i].setShader(shader);
    }
    m_elapsed_timer.start();
}

void lcf::ShaderToy::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
    for (auto &buffer : m_buffers) {
        buffer.setSize(width, height);
    }
    iResolution.setX(width);
    iResolution.setY(height);
    iFrame = 0;
}

void lcf::ShaderToy::setBuffer(int index, const BufferTextureList &textures)
{
    auto &buffer = m_buffers[index];
    for (auto texture : textures) {
        std::visit([&](auto &&arg) {
            if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, int>) {
                buffer.addTexture(&m_buffers[arg]);
            } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, SharedGLTexturePtr>) {
                buffer.addTexture(arg);
            }
        }, texture);
    }
}

void lcf::ShaderToy::update()
{
    static GLint original_viewport[4];
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glGetIntegerv(GL_VIEWPORT, original_viewport);
    gl->glViewport(0, 0, m_width, m_height);
    for (auto &buffer : m_buffers) {
        if (not buffer.shader()) { qDebug() << "No shader for buffer"; return; }
        GLHelper::setShaderUniforms(buffer.shader(), {
            {"iTime", utils::elapsed_time_s()},
            {"iFrame", iFrame},
        });
        buffer.update();
    }
    gl->glViewport(original_viewport[0], original_viewport[1], original_viewport[2], original_viewport[3]);
    ++iFrame;
}

GLuint lcf::ShaderToy::texture() const
{
    return m_buffers.back().texture();
}

void lcf::ShaderToy::bind(uint unit)
{
    if (m_buffers.empty()) {
        qDebug() << "No buffer to bind";
        return;
    }
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0 + unit);
    gl->glBindTexture(GL_TEXTURE_2D, this->texture());
}

void lcf::ShaderToy::release()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    gl->glActiveTexture(GL_TEXTURE0);
    gl->glBindTexture(GL_TEXTURE_2D, 0);
}
