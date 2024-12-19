#include "GLFrameBufferObject.h"

namespace lcf {
    class FrameBufferObject
    {
    public:
        using UniquePtr = std::unique_ptr<FrameBufferObject>;
        FrameBufferObject(int width, int height);
        bool bind();
        bool release();
        void addDepthTexture();
        void addColorTexture();
        unsigned int takeDepthTexture(int index = 0);
        unsigned int takeColorTexture(int index = 0);
    private:
        int m_original_viewport[4];
        UniqueGLFrameBufferObjectPtr m_fbo;
        std::vector<unsigned int> m_depth_textures;
        std::vector<unsigned int> m_color_textures;
    };
}