#pragma once

#include "TextureWrapper.h"
#include <vector>
#include "GLShaderProgram.h"
#include "Image.h"

namespace lcf {
    class Material
    {
        using TextureList = std::vector<TextureWrapper>;
    public:
        Material() = default;
        Material(const Material&);
        void addTexture(TextureWrapper texture);
        void addData(unsigned char* data, int width, int height);
        void addImageData(const Image& image);
        void addImageData(Image&& image);
        void create();
        void bind();
        void release();
    private:
        TextureList m_textures;
        bool m_created = false;
        std::vector<Image> m_image_data;
    };
}