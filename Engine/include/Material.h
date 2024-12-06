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
        using SharedPtr = std::shared_ptr<Material>;
        static SharedPtr newShared();
        static SharedPtr newCreatedShared();
        Material() = default;
        Material(const Material&);
        ~Material() = default;
        void addTexture(TextureWrapper texture);
        void addTextures(const TextureList& textures);
        void addData(unsigned char* data, int width, int height);
        void addImageData(const Image& image);
        void addImageData(Image&& image);
        const TextureList &textures() const;
        void setTexture(int index, TextureWrapper texture);
        void create();
        bool isCreated() const;
        void bind();
        void release();
    private:
        TextureList m_textures;
        bool m_created = false;
        std::vector<Image> m_image_data;
    };
}