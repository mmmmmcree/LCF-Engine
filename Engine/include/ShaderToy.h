#pragma once

#include "GLHelper.h"
#include <QOpenGLContext>
#include "ShaderToyBuffer.h"
#include <QElapsedTimer>
#include <vector>
#include "Matrix.h"
#include "NativeTextureWrapper.h"
#include "TextureWrapper.h"

namespace lcf {
    class ShaderToy
    {
    public:
        using BufferTexture = std::variant<int, SharedGLTexturePtr>; // int for binding buffer as a texture, QOpenGLTexture * for using normal texture
        using BufferTextureList = std::vector<BufferTexture>;
        using ShaderToyBufferList = std::vector<ShaderToyBuffer>;
        using ShaderList = std::vector<GLShaderProgram::UniquePtr>;
        using SharedPtr = std::shared_ptr<ShaderToy>;
        ShaderToy(int width, int height, const QStringList &frag_paths);
        void setSize(int width, int height);
        void setBuffer(int index, const BufferTextureList &buffer_texture_list);
        void update();
        NativeTextureWrapper texture() const;
    private:
        int m_width, m_height;
        ShaderToyBufferList m_buffers;
        ShaderList m_shaders;
    private:
        QElapsedTimer m_elapsed_timer;
        Vector3D iResolution;
        int iFrame = 0;
    };
}
