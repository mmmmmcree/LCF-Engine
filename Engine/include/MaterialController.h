#pragma once

#include "TextureWrapper.h"
#include "Image.h"
#include "PhongMaterial.h"
#include "Define.h"
#include "UserCustomMaterial.h"

namespace lcf {
    class MaterialController
    {
        friend class AssimpLoader;
    public:
        using TextureInfoMap = std::map<int, TextureWrapper>;
        using TextureDataInfo = std::pair<int, Image>;
        using TextureDataInfoList = std::vector<TextureDataInfo>;
        using SharedPtr = std::shared_ptr<MaterialController>;
        MaterialController();
        // void setMaterial(const Material::SharedPtr &material);
        const Material::SharedPtr &material() const;
        void setTexture(int texture_type, TextureWrapper texture);
        const TextureInfoMap &textureInfoMap() const;
        void setTextures(const TextureInfoMap& texture_info_map);
        void create();
        void bind();
        void release();
        const UniformList &asUniformList() const;
        void setType(MaterialType type);
        MaterialType materialType() const;
    private:
        void setImageData(int texture_type, unsigned char* data, int width, int height);
        void setImageData(int texture_type, const Image& image);
        void setImageData(int texture_type, Image&& image);
        void updateMaterial();
        PhongMaterial::UniquePtr generatePhongMaterial();
        UserCustomMaterial::UniquePtr generateUserCustomMaterial();
    private:
        MaterialType m_material_type = MaterialType::Phong;
        TextureInfoMap m_textures;
        TextureDataInfoList m_image_data;
        Material::SharedPtr m_material = nullptr;
        float m_shininess = 32.0f;
    };
}