#pragma once

#include "Scene.h"
#include "MSAAFBO.h"
#include "ScreenFBO.h"
#include "GLFunctions.h"
#include <functional>
#include "Bloomer.h"
#include <QOpenGLContext>
#include <QSurface>
#include <QObject>
#include "SingleUniform.h"

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
        void setHDRExposure(float exposure);
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
        MSAAFBO::UniquePtr m_msaa_fbo;  
        ScreenFBO::UniquePtr m_post_process_fbo;
        GLShaderProgram::SharedPtr m_post_process_shader;
        Bloomer::UniquePtr m_bloomer;
        std::function<void(Scene *scene)> m_render_pass_procedure;
        std::function<void()> m_post_process_procedure;
        SingleUniform<bool> m_gamma_correction_enabled = true;
        SingleUniform<bool> m_hdr_enabled = true;
        bool m_bloom_enabled = true;
        bool m_msaa_enabled = true;
        SingleUniform<float> m_hdr_exposure = 1.0f;
    };
}