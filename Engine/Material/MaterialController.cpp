#include "MaterialController.h"
#include "Define.h"
#include "GLHelper.h"
#include "ControlManager.h"
#include "TextureDispatcher.h"
#include "ShaderManager.h"

lcf::PhongMaterial::UniquePtr lcf::MaterialController::generatePhongMaterial()
{
    auto material = PhongMaterial::UniquePtr(new lcf::PhongMaterial);
    auto iter = m_textures.find(TextureType::Diffuse);
    if (iter != m_textures.end()) {
        material->setDiffuseMap(iter->second);
    }
    iter = m_textures.find(TextureType::Specular);
    if (iter != m_textures.end()) {
        material->setSpecularMap(iter->second);
    }
    iter = m_textures.find(TextureType::Normal);
    if (iter != m_textures.end()) {
        material->setNormalMap(iter->second);
    }
    return material;
}

lcf::PBRMaterial::UniquePtr lcf::MaterialController::generatePBRMaterial()
{
    auto material = PBRMaterial::UniquePtr(new lcf::PBRMaterial);
    auto iter = m_textures.find(TextureType::BaseColor);
    if (iter != m_textures.end()) {
        material->setAlbedoMap(iter->second);
    }
    iter = m_textures.find(TextureType::Metalness);
    if (iter != m_textures.end()) {
        material->setMetallicMap(iter->second);
    }
    iter = m_textures.find(TextureType::Roughness);
    if (iter != m_textures.end()) {
        material->setRoughnessMap(iter->second);
    }
    iter = m_textures.find(TextureType::GltfMetallicRoughness);
    if (iter != m_textures.end()) {
        material->setMetallicMap(iter->second);
        material->setRoughnessMap(iter->second);
    }
    iter = m_textures.find(TextureType::Normal);
    if (iter != m_textures.end()) {
        material->setNormalMap(iter->second);
    }
    iter = m_textures.find(TextureType::Emissive);
    if (iter != m_textures.end()) {
        material->setEmissiveMap(iter->second);
    }
    iter = m_textures.find(TextureType::AmbientOcclusion);
    if (iter != m_textures.end()) {
        material->setAOMap(iter->second);
    }
    iter = m_textures.find(TextureType::Lightmap);
    if (iter != m_textures.end()) {
        material->setAOMap(iter->second);
    }
    return material;
}

lcf::UserCustomMaterial::UniquePtr lcf::MaterialController::generateUserCustomMaterial()
{
    auto material = UserCustomMaterial::UniquePtr(new lcf::UserCustomMaterial(&m_textures));
    return material;
}

lcf::IBLMaterial::UniquePtr lcf::MaterialController::generateIBLMaterial()
{
    auto material = IBLMaterial::createUnique();
    auto iter = m_textures.find(TextureType::IBLIrradiance);
    if (iter != m_textures.end()) {
        material->setIrradianceMap(iter->second);
    }
    iter = m_textures.find(TextureType::IBLPrefilter);
    if (iter != m_textures.end()) {
        material->setPrefilterMap(iter->second);
    }
    iter = m_textures.find(TextureType::IBLBRDF);
    if (iter != m_textures.end()) {
        material->setBRDFMap(iter->second);
    }
    return material;
}

const std::string &lcf::MaterialController::textureTypeToString(TextureType type) const
{
    static std::unordered_map<TextureType, std::string> texture_type_to_string = {
        {TextureType::None, "None"},
        {TextureType::Diffuse, "material.diffuse_map"},
        {TextureType::Specular, "material.specular_map"},
        {TextureType::Ambient, "material.ambient_map"},
        {TextureType::Emissive, "material.emissive_map"},
        {TextureType::Height, "material.height_map"},
        {TextureType::Normal, "material.normal_map"},
        {TextureType::Shininess, "material.shininess_map"},
        {TextureType::Opacity, "material.opacity_map"},
        {TextureType::Displacement, "material.displacement_map"},
        {TextureType::Lightmap, "material.lightmap_map"},
        {TextureType::Reflection, "material.reflection_map"},
        {TextureType::BaseColor, "material.base_color_map"},
        {TextureType::NormalCamera, "material.normal_camera_map"},
        {TextureType::EmissionColor, "material.emission_color_map"},
        {TextureType::Metalness, "material.metallic_map"},
        {TextureType::Roughness, "material.roughness_map"},
        {TextureType::AmbientOcclusion, "material.ambient_occlusion_map"},
        {TextureType::Unknown, "material.unknown_map"},
        {TextureType::Sheen, "material.sheen_map"},
        {TextureType::ClearCoat, "material.clear_coat_map"},
        {TextureType::Transmission, "material.transmission_map"},
        {TextureType::MayaBaseColor, "material.maya_base_color_map"},
        {TextureType::MayaSpecular, "material.maya_specular_map"},
        {TextureType::MayaSpecularColor, "material.maya_specular_color_map"},
        {TextureType::MayaSpecularRoughness, "material.maya_specular_roughness_map"},
        {TextureType::Anisotropy, "material.anisotropy_map"},
        {TextureType::GltfMetallicRoughness, "material.gltf_metallic_roughness_map"},
        {TextureType::IBLIrradiance, "ibl_material.irradiance_map"},
        {TextureType::IBLPrefilter, "ibl_material.prefilter_map"},
        {TextureType::IBLBRDF, "ibl_material.brdf_map"},
        {TextureType::UserCustom0, "channel0"},
        {TextureType::UserCustom1, "channel1"},
        {TextureType::UserCustom2, "channel2"},
        {TextureType::UserCustom3, "channel3"},
        {TextureType::UserCustom4, "channel4"},
        {TextureType::UserCustom5, "channel5"},
        {TextureType::UserCustom6, "channel6"},
        {TextureType::UserCustom7, "channel7"},
        {TextureType::UserCustom8, "channel8"},
        {TextureType::UserCustom9, "channel9"},
        {TextureType::UserCustom10, "channel10"},
        {TextureType::UserCustom11, "channel11"},
        {TextureType::UserCustom12, "channel12"},
        {TextureType::UserCustom13, "channel13"},
        {TextureType::UserCustom14, "channel14"},
        {TextureType::UserCustom15, "channel15"}
    };
    return texture_type_to_string.at(type);
}

lcf::MaterialController::MaterialController()
{
}

lcf::MaterialController::MaterialController(const MaterialController &other)
{
    m_textures = other.m_textures;
    m_material_type = other.m_material_type;
    m_shader = other.m_shader;
    this->updateMaterial();
}

lcf::MaterialController::SharedPtr lcf::MaterialController::createShared()
{
    return std::make_shared<lcf::MaterialController>();
}

lcf::MaterialController::SharedPtr lcf::MaterialController::createShared(const MaterialController &other)
{
    return std::make_shared<lcf::MaterialController>(other);
}

bool lcf::MaterialController::isValid() const
{
    return m_shader.get();
}

void lcf::MaterialController::setTexture(int texture_type, TextureWrapper texture)
{
    auto iter = m_textures.find(texture_type);
    if (iter == m_textures.end()) {
        m_textures.emplace(std::make_pair(texture_type, texture));
    } else {
        iter->second = texture;
    }
    this->updateMaterial();
}

const lcf::MaterialController::TextureInfoMap &lcf::MaterialController::textureInfoMap() const
{
    return m_textures;
}

void lcf::MaterialController::setTextures(const TextureInfoMap &texture_info_map)
{
    for (const auto & [type, texture] : texture_info_map) {
        auto iter = m_textures.find(type);
        if (iter != m_textures.end()) {
            iter->second = texture;
        } else {
            m_textures.emplace(std::make_pair(type, texture));
        }
    }
    this->updateMaterial();
}

void lcf::MaterialController::create()
{
    for (const auto & [type, image] : m_image_data) {
        auto iter = m_textures.find(type);
        if (iter != m_textures.end()) { continue; }
        auto texture = GLHelper::generateTextureByTextureType(static_cast<TextureType>(type), *image);
        m_textures.emplace(std::make_pair(type, texture));
    }
    m_image_data.clear();
    this->deduceMaterialType();
    this->updateMaterial();
}

bool lcf::MaterialController::isCreated() const
{
    return m_image_data.empty();
}

void lcf::MaterialController::bind()
{
    if (not m_material) { this->updateMaterial(); }
    m_material->dispatch();
    if (m_shader) { m_shader->bindWithTextures(); }
}

void lcf::MaterialController::release()
{
    if (not m_material) { return; }
    if (m_shader) { m_shader->release(); }
}

void lcf::MaterialController::setMaterialType(MaterialType type)
{
    m_material_type = type;
    this->updateMaterial();
}

lcf::MaterialType lcf::MaterialController::materialType() const
{
    return m_material_type;
}

void lcf::MaterialController::setShader(const GLShaderProgram::SharedPtr & shader)
{
    m_shader = shader;
}

const lcf::GLShaderProgram::SharedPtr & lcf::MaterialController::shader() const
{
    return m_shader;
}

void lcf::MaterialController::setImageData(int type, const SharedImagePtr &image)
{
    if (not image or image->isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(type, image));
}

void lcf::MaterialController::updateMaterial()
{
    switch (m_material_type) {
        case MaterialType::None : { m_material = Material::UniquePtr(new Material); }
        case MaterialType::Phong : { m_material = this->generatePhongMaterial(); } break;
        case MaterialType::UserCustom : { m_material = this->generateUserCustomMaterial(); } break;
        case MaterialType::PBR : { m_material = this->generatePBRMaterial(); } break;
        case MaterialType::IBL : { m_material = this->generateIBLMaterial(); } break;
    }
}

void lcf::MaterialController::deduceMaterialType()
{
    if (m_material_type != MaterialType::None) { return; }
    if (m_textures.find(TextureType::UserCustom0) != m_textures.end()) {
        m_material_type = MaterialType::UserCustom;
    }
    if (m_textures.find(TextureType::Diffuse) != m_textures.end()) {
        m_material_type = MaterialType::Phong;
    }
    if (m_textures.find(TextureType::Metalness) != m_textures.end()
        or m_textures.find(TextureType::Roughness) != m_textures.end()) {
        m_material_type = MaterialType::PBR;
    }
}
