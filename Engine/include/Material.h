#pragma once

#include "TextureWrapper.h"
#include "GLShaderProgram.h"
#include "Image.h"
#include <vector>
#include <map>

namespace lcf {
    class AssimpLoader;
    class Material
    {
        friend class AssimpLoader;
    public:
        using TextureInfoMap = std::map<int, TextureWrapper>;
        using TextureDataInfo = std::pair<int, Image>;
        using TextureDataInfoList = std::vector<TextureDataInfo>;
        using SharedPtr = std::shared_ptr<Material>;
        static SharedPtr newShared();
        Material() = default;
        Material(const Material&);
        ~Material() = default;
        void setTexture(int texture_type, TextureWrapper texture);
        const TextureInfoMap &textureInfoMap() const;
        void setTextures(const TextureInfoMap& texture_info_map);
        void create();
        void bind(); //- 纹理绑定的纹理单元顺序为TextureType枚举值的递增顺序
        void release();
    private:
        void setImageData(int texture_type, unsigned char* data, int width, int height);
        void setImageData(int texture_type, const Image& image);
        void setImageData(int texture_type, Image&& image);
    private:
        TextureInfoMap m_textures;
        TextureDataInfoList m_image_data;
    };
}