#pragma once

#include "Scene.h"
#include "MSAAFBO.h"
#include "ScreenFBO.h"
#include "GLFunctions.h"
#include "ShaderUniformBinder.h" 

namespace lcf {
    class Renderer
    {
    public:
        static Renderer *instance();
        void setRenderSize(int width, int height);
        void render(Scene *scene);
        void enableHDR(bool enable);
        void enableMSAA(bool enable);
    private:
        Renderer();
    private:
        MSAAFBO::UniquePtr m_msaa_fbo;  
        ScreenFBO::UniquePtr m_post_process_fbo;
        // SharedGLShaderProgramPtr m_post_process_shader;
        ShaderUniformBinder::SharedPtr m_post_process_shader_binder;
        bool m_hdr_enabled = false;
        bool m_msaa_enabled = false;
    };
}