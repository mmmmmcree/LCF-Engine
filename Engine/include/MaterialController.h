#pragma once

#include "TextureWrapper.h"
#include "Image.h"
#include "PhongMaterial.h"
#include "Define.h"
#include "UserCustomMaterial.h"
#include "PBRMaterial.h"
#include "ShaderUniformBinder.h"

namespace lcf {
    class MaterialController
    {
        friend class AssimpLoader;
    public:
        using TextureInfoMap = std::map<int, TextureWrapper>;
        using TextureDataInfo = std::pair<int, SharedImagePtr>;
        using TextureDataInfoList = std::vector<TextureDataInfo>;
        using SharedPtr = std::shared_ptr<MaterialController>;
        MaterialController() = default;
        static SharedPtr createShared();
        bool isValid() const;
        void setTexture(int texture_type, TextureWrapper texture);
        const TextureInfoMap &textureInfoMap() const;
        void setTextures(const TextureInfoMap& texture_info_map);
        void create();
        bool isCreated() const;
        void bind();
        void release();
        UniformList asUniformList() const;
        void setMaterialType(MaterialType type);
        MaterialType materialType() const;
        void setShininess(float shininess);
        void setShader(const SharedGLShaderProgramPtr& shader);
        GLShaderProgram *shader() const;
        void copyShaderUniformBinderFrom(const MaterialController *other);
        void setShaderUniformBinder(const ShaderUniformBinder::SharedPtr& su_binder);
        const ShaderUniformBinder::SharedPtr& shaderUniformBinder() const;
    private:
        void setImageData(int type, const SharedImagePtr &image);
        void updateMaterial();
        PhongMaterial::UniquePtr generatePhongMaterial();
        PBRMaterial::UniquePtr generatePBRMaterial();
        UserCustomMaterial::UniquePtr generateUserCustomMaterial();
    private:
        ShaderUniformBinder::SharedPtr m_shader_uniform_binder = ShaderUniformBinder::createShared();
        MaterialType m_material_type = MaterialType::Phong;
        TextureInfoMap m_textures;
        TextureDataInfoList m_image_data;
        Material::UniquePtr m_material = nullptr;
        float m_shininess = 32.0f;
    };
}