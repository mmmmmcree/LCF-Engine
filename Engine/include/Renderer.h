#pragma once

#include "Scene.h"
#include "MSAAFBO.h"
#include "ScreenFBO.h"
#include "GLFunctions.h"
#include "ShaderUniformBinder.h" 
#include <functional>
#include "Bloomer.h"
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QObject>

namespace lcf {
    class Renderer : public QObject
    {
        Q_OBJECT
    public:
        static Renderer *instance();
        void initialize(QOpenGLContext *context);
        void setRenderSize(int width, int height);
        void render(Scene *scene);
        void enableHDR(bool enable);
        bool isHDREnabled() const;
        void enableMSAA(bool enable);
        bool isMSAAEnabled() const;
        void enableBloom(bool enable);
        bool isBloomEnabled() const;
        void enableGammaCorrection(bool enable);
        bool isGammaCorrectionEnabled() const;
    private:
        Renderer() = default;
        void updateRenderPassProcedure();
        void updatePostProcessProcedure();
    signals:
        void HDREnabledChanged(bool enabled);
        void MSAAEnabledChanged(bool enabled);
        void BloomEnabledChanged(bool enabled);
        void GammaCorrectionEnabledChanged(bool enabled);
    private:
        QOpenGLContext *m_context = nullptr;
        QOffscreenSurface *m_surface = nullptr;
        MSAAFBO::UniquePtr m_msaa_fbo;  
        ScreenFBO::UniquePtr m_post_process_fbo;
        ShaderUniformBinder::SharedPtr m_post_process_shader_binder;
        Bloomer::UniquePtr m_bloomer;
        std::function<void(Scene *scene)> m_render_pass_procedure;
        std::function<void()> m_post_process_procedure;
        bool m_gamma_correction_enabled = true;
        bool m_hdr_enabled = true;
        bool m_bloom_enabled = true;
        bool m_msaa_enabled = true;
    };
}