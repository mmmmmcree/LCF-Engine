#pragma once

#include "Scene.h"
#include "MSAAFBO.h"
#include "RegularFBO.h"
#include "HDRFBO.h"
#include "GLFunctions.h"

namespace lcf {
    class Renderer
    {
    public:
        static Renderer *instance();
        void setRenderSize(int width, int height);
        void render(Scene *scene);
    private:
        Renderer();
    private:
        MSAAFBO::UniquePtr m_msaa_fbo;  
        HDRFBO::UniquePtr m_post_process_fbo;
        SharedGLShaderProgramPtr m_post_process_shader;
    };
}