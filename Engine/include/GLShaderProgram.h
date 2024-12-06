#pragma once

#include <QOpenGLShaderProgram>
#include <memory>

namespace lcf {
    using GLShaderProgram = QOpenGLShaderProgram;

    using SharedGLShaderProgramPtr = std::shared_ptr<GLShaderProgram>;

    using UniqueGLShaderProgramPtr = std::unique_ptr<GLShaderProgram>;
}