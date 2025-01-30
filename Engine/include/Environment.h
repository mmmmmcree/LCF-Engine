#pragma once
#include "Mesh.h"


namespace lcf {
    class Environment
    {
    public:
        Environment();
        void setSkyboxTexture(TextureWrapper texture);
    private:
        Mesh::UniquePtr m_skybox;
    };
}