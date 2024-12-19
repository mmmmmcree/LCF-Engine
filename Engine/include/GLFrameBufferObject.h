#pragma once

#include <QOpenGLFramebufferObject>
#include <memory>

namespace lcf {
    using GLFrameBufferObject = QOpenGLFramebufferObject;

    using GLFrameBufferObjectFormat = QOpenGLFramebufferObjectFormat;

    using UniqueGLFrameBufferObjectPtr = std::unique_ptr<GLFrameBufferObject>;
}