#pragma once

#include <QOpenGLFunctions_4_5_Core>

namespace lcf {
    struct GLFunctions
    {
        static QOpenGLFunctions_4_5_Core* getGLFunctionsFromCurrentContext();
    };
}