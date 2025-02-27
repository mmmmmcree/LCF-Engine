#include "GLFunctions.h"
#include <QOpenGLVersionFunctionsFactory>

QOpenGLFunctions_4_5_Core *lcf::GLFunctions::getGLFunctionsFromCurrentContext()
{
    return QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(QOpenGLContext::currentContext());
}

// QOpenGLFunctions_4_5_Core *lcf::GLFunctions::getGLFunctionsFromContext(QOpenGLContext *context)
// {
//     return QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_4_5_Core>(QOpenGLContext::currentContext());
// }
