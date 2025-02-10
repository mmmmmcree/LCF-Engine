#pragma once
#include "Mesh.h"
#include "MaterialController.h"

namespace lcf {
    class Environment
    {
    public:
        Environment();
        void setSkyboxTexture(TextureWrapper texture);
        void drawSkybox();
        void bind(int start_location = 0);
        void release();
    private:
        void translateSkyboxTextureToCubemap();
        TextureWrapper fromSphereToCube(TextureWrapper source);
        TextureWrapper IBLConvolution(TextureWrapper environment_cubemap);
        TextureWrapper IBLPrefilter(TextureWrapper environment_cubemap);
        TextureWrapper IBLBRDF();
    private:
        Mesh::UniquePtr m_skybox;
        TextureWrapper m_skybox_texture;
        MaterialController m_material_controller;
    };
}