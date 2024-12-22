#pragma once

#include <QOpenGLTexture>
#include <memory>

namespace lcf {
    using GLTexture = QOpenGLTexture;
    
    using SharedGLTexturePtr = std::shared_ptr<GLTexture>;

    using UniqueGLTexturePtr = std::unique_ptr<GLTexture>;
}

