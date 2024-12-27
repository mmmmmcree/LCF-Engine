#pragma once

#include "Scene.h"
#include "MSAAFBO.h"
#include "ScreenFBO.h"
#include "GLFunctions.h"
#include "ShaderUniformBinder.h" 
#include <functional>

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
        void updateRenderPassFunction();
    private:
        MSAAFBO::UniquePtr m_msaa_fbo;  
        ScreenFBO::UniquePtr m_post_process_fbo;
        ShaderUniformBinder::SharedPtr m_post_process_shader_binder;
        std::function<void(Scene *scene)> m_render_pass_func;
        bool m_hdr_enabled = false;
        bool m_msaa_enabled = false;
    };
}