#include "GLFunctions.h"

QOpenGLFunctions_4_5_Core *lcf::GLFunctions::global()
{
    static QOpenGLFunctions_4_5_Core s_glf;
    return &s_glf;
}