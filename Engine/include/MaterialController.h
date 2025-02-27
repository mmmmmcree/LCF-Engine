#pragma once

#include "TextureWrapper.h"
#include "Image.h"
#include "PhongMaterial.h"
#include "Define.h"
#include "UserCustomMaterial.h"
#include "PBRMaterial.h"
#include "IBLMaterial.h"
#include "GLShaderProgram.h"

namespace lcf {
    class MaterialController
    {
        friend class AssimpLoader;
    public:
        using TextureInfoMap = std::map<int, TextureWrapper>;
        using TextureDataInfo = std::pair<int, SharedImagePtr>;
        using TextureDataInfoList = std::vector<TextureDataInfo>;
        using SharedPtr = std::shared_ptr<MaterialController>;
        MaterialController();
        MaterialController(const MaterialController &other);
        static SharedPtr createShared();
        static SharedPtr createShared(const MaterialController &other);
        bool isValid() const;
        void setTexture(int texture_type, TextureWrapper texture);
        const TextureInfoMap &textureInfoMap() const;
        void setTextures(const TextureInfoMap& texture_info_map);
        void create();
        bool isCreated() const;
        void bind();
        void release();
        void setMaterialType(MaterialType type);
        MaterialType materialType() const;
        void setShader(const GLShaderProgram::SharedPtr& shader);
        const GLShaderProgram::SharedPtr & shader() const;
    private:
        void setImageData(int type, const SharedImagePtr &image);
        void updateMaterial();
        void deduceMaterialType();
        PhongMaterial::UniquePtr generatePhongMaterial();
        PBRMaterial::UniquePtr generatePBRMaterial();
        UserCustomMaterial::UniquePtr generateUserCustomMaterial();
        IBLMaterial::UniquePtr generateIBLMaterial();
        const std::string &textureTypeToString(TextureType type) const;
    private:
        GLShaderProgram::SharedPtr m_shader;
        MaterialType m_material_type = MaterialType::None;
        TextureInfoMap m_textures;
        TextureDataInfoList m_image_data;
        Material::UniquePtr m_material;
    };
}