#pragma once

#include "Obejct3D.h"

namespace lcf {
    class Scene : public Object3D
    {
    public:    
        Scene() = default;
        static Scene *global();
        static Scene *current();
        static void setCurrent(Scene *scene);
    private:
        inline static Scene *s_current = nullptr;
    };
}