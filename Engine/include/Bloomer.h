#pragma once

#include "SingleColorAttachmentFBO.h"
#include "ScreenFBO.h"
#include "ShaderUniformBinder.h"
#include <array>
#include "MyUniform.h"

namespace lcf {
    class Bloomer
    {
    public:
        using UniquePtr = std::unique_ptr<Bloomer>;
        static UniquePtr createUnique(int width, int height);
        Bloomer(int width, int height);
        void bloom(FrameBufferObject *source);
        const NativeTextureWrapper &texture() const;
    private:
        void extract(FrameBufferObject *source, FrameBufferObject *dest);
        void downSample(FrameBufferObject *source, FrameBufferObject *dest);
        void upSample(FrameBufferObject *low, FrameBufferObject *high, FrameBufferObject *dest);
        void merge(FrameBufferObject *source, FrameBufferObject *bloom, FrameBufferObject *dest);
    private:
        inline static const int s_sample_count = 5;
        inline static const int s_min_resolution = 32;
        std::array<SingleColorAttachmentFBO::SharedPtr, s_sample_count> m_down_sample_fbo_list;
        std::array<SingleColorAttachmentFBO::SharedPtr, s_sample_count - 1> m_up_sample_fbo_list;
        ScreenFBO::UniquePtr m_screen_fbo;
        SingleColorAttachmentFBO::SharedPtr m_last_sample_fbo;
        GLShaderProgram::SharedPtr m_extract_shader;
        GLShaderProgram::SharedPtr m_up_sample_shader;
        GLShaderProgram::SharedPtr m_merge_shader;
        MySingleUniform m_threshold = MySingleUniform(10.0f);
        MySingleUniform m_bloom_intensity = MySingleUniform(0.5f);
        MySingleUniform m_bloom_attenuation = MySingleUniform(1.0f);
        MySingleUniform m_bloom_radius = MySingleUniform(0.005f);
    };
}