#include "MaterialController.h"
#include "Define.h"
#include "GLHelper.h"
#include "ControlManager.h"

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
    material->setShininess(m_shininess);
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
    return material;
}

lcf::UserCustomMaterial::UniquePtr lcf::MaterialController::generateUserCustomMaterial()
{
    auto material = UserCustomMaterial::UniquePtr(new lcf::UserCustomMaterial(&m_textures));
    return material;
}

lcf::MaterialController::SharedPtr lcf::MaterialController::createShared()
{
    return std::make_shared<lcf::MaterialController>();
}

bool lcf::MaterialController::isValid() const
{
    return m_shader_uniform_binder->hasShader();
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
    this->updateMaterial();
}

bool lcf::MaterialController::isCreated() const
{
    return m_image_data.empty();
}

void lcf::MaterialController::bind()
{
    if (not m_material) { this->updateMaterial(); }
    m_material->bind();
    m_shader_uniform_binder->bind();
}

void lcf::MaterialController::release()
{
    if (not m_material) { return; }
    m_material->release();
    m_shader_uniform_binder->release();
}

lcf::UniformList lcf::MaterialController::asUniformList() const
{
    if (not m_material) { return lcf::UniformList{}; }
    return m_material->asUniformList();
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

void lcf::MaterialController::setShininess(float shininess)
{
    m_shininess = std::max(shininess, std::numeric_limits<float>::min()); //! shinness <= 0.0f在计算镜面反射出现bug
}

void lcf::MaterialController::setShader(const SharedGLShaderProgramPtr & shader)
{
    m_shader_uniform_binder->setShader(shader);
}

lcf::GLShaderProgram * lcf::MaterialController::shader() const
{
    return m_shader_uniform_binder->shader().get();
}

void lcf::MaterialController::copyShaderUniformBinderFrom(const MaterialController *other)
{
    m_shader_uniform_binder = ShaderUniformBinder::createShared(*other->m_shader_uniform_binder);
}

void lcf::MaterialController::setShaderUniformBinder(const ShaderUniformBinder::SharedPtr &su_binder)
{
    if (not su_binder) { return; }
    m_shader_uniform_binder = su_binder;
}

const lcf::ShaderUniformBinder::SharedPtr &lcf::MaterialController::shaderUniformBinder() const
{
    return m_shader_uniform_binder;
}

void lcf::MaterialController::setImageData(int type, const SharedImagePtr &image)
{
    if (not image or image->isNull()) { return; }
    m_image_data.emplace_back(std::make_pair(type, image));
}

void lcf::MaterialController::updateMaterial()
{
    switch (m_material_type) {
        case Phong : { m_material = this->generatePhongMaterial(); } break;
        case UserCustom : { m_material = this->generateUserCustomMaterial(); } break;
        case PBR : { m_material = this->generatePBRMaterial(); } break;
    }
    m_shader_uniform_binder->setUniforms(m_material->asUniformList());
}
