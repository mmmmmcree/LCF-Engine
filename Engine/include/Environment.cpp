#include "Environment.h"
#include "Geometry.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "GLHelper.h"
#include "Constants.h"

lcf::Environment::Environment()
{
    m_skybox = Mesh::createUnique(Geometry::cube());
    m_skybox->setShader(ShaderManager::instance()->get(ShaderManager::CubicSkybox));
    m_skybox->setMaterialType(MaterialType::UserCustom);
}

void lcf::Environment::setSkyboxTexture(TextureWrapper texture)
{
    if (not texture.isValid()) { return; } 
    texture.setWrapMode(GLTexture::ClampToEdge);
    texture.setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
    m_skybox->setTexture(TextureType::UserCustom0, texture);
    m_skybox_texture = texture;
}

void lcf::Environment::drawSkybox()
{
    if (m_skybox_texture.target() == GLTexture::Target2D) {
        //- 转换涉及到draw函数的调用，需要在渲染线程中进行，故放在drawSkybox函数中
        this->translateSkyboxTextureToCubemap();
    }
    m_skybox->draw();
}

void lcf::Environment::bind()
{
    m_material_controller.bind();
}

void lcf::Environment::release()
{
    m_material_controller.release();
}

void lcf::Environment::translateSkyboxTextureToCubemap()
{
    m_skybox_texture = this->fromSphereToCube(m_skybox_texture);
    m_skybox_texture.setWrapMode(GLTexture::ClampToEdge);
    m_skybox_texture.setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
    m_skybox->setTexture(TextureType::UserCustom0, m_skybox_texture);
    if (GLHelper::isFloatingPointTextureFormat(m_skybox_texture.format())) {
        m_material_controller.setTexture(TextureType::IBLIrradiance, this->IBLConvolution(m_skybox_texture));
        m_material_controller.setTexture(TextureType::IBLPrefilter, this->IBLPrefilter(m_skybox_texture));
        m_material_controller.setTexture(TextureType::IBLBRDF, this->IBLBRDF());
        m_material_controller.setMaterialType(MaterialType::IBL);
    } else {
        m_material_controller.setMaterialType(MaterialType::None);
    }
}

lcf::TextureWrapper lcf::Environment::fromSphereToCube(TextureWrapper source)
{
    const int width = 1024;
    FrameBufferObject fbo(width, width);
    auto color_attachment = GLHelper::generateCubeMapAttachment(width, source.format());
    color_attachment.setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
    color_attachment.setWrapMode(GLTexture::ClampToEdge);
    fbo.addColorAttachment(color_attachment);
    auto shader = ShaderManager::instance()->get(ShaderManager::SphereToCube);
    fbo.bind();
    shader->bindWithTextures();
    source.bind(0);
    Geometry::cube()->draw();
    source.release(0);
    shader->release();
    fbo.release();
    return TextureWrapper(fbo.colorAttachment());    
}

lcf::TextureWrapper lcf::Environment::IBLConvolution(TextureWrapper environment_cubemap)
{
    const int width = 32;
    FrameBufferObject fbo(width, width);
    auto color_attachment = GLHelper::generateCubeMapAttachment(width, GLTexture::TextureFormat::RGB32F);
    color_attachment.setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
    color_attachment.setWrapMode(GLTexture::ClampToEdge);
    fbo.addColorAttachment(color_attachment);
    auto shader = ShaderManager::instance()->get(ShaderManager::IBLConvolution);
    fbo.bind();
    shader->bindWithTextures();
    environment_cubemap.bind(0);
    Geometry::cube()->draw();
    auto gl = QOpenGLContext::currentContext()->functions();
    environment_cubemap.release(0);
    shader->release();
    fbo.release();
    return TextureWrapper(fbo.colorAttachment());    
}

lcf::TextureWrapper lcf::Environment::IBLPrefilter(TextureWrapper environment_cubemap)
{
    const int width = 128;
    FrameBufferObject fbo(width, width);
    auto color_attachment = GLHelper::generateCubeMapAttachment(width, GLTexture::TextureFormat::RGB16F);
    color_attachment.setMinMagFilters(GLTexture::LinearMipMapLinear, GLTexture::Linear);
    color_attachment.setWrapMode(GLTexture::ClampToEdge);
    color_attachment.generateMipMaps();
    fbo.addColorAttachment(color_attachment);
    auto shader = ShaderManager::instance()->get(ShaderManager::IBLPrefilter);
    shader->bindWithTextures();
    environment_cubemap.bind(0);
    int mip_levels = 5;
    auto gl = QOpenGLContext::currentContext()->extraFunctions();
    for (int i = 0; i < mip_levels; ++i) {
        shader->setUniformValue("roughness", static_cast<float>(i) / (mip_levels - 1));
        fbo.setMipMapLevel(i);
        fbo.bind();
        Geometry::cube()->draw();
        fbo.release();
    }
    environment_cubemap.release(0);
    shader->release();
    return color_attachment; 
}

lcf::TextureWrapper lcf::Environment::IBLBRDF()
{
    auto tex = TextureManager::instance()->loadTexture2D(path::res_prefix + "BRDF.hdr", false, false);
    tex->setMinMagFilters(GLTexture::Linear, GLTexture::Linear);
    tex->setWrapMode(GLTexture::ClampToEdge);
    return tex;
    // const int width = 512;
    // bool current_context = QOpenGLContext::currentContext();
    // if (not current_context) { m_context->makeCurrent(m_surface); }
    // NativeTextureWrapper tex = GLHelper::generateTexture2DAttachment(width, width, GLTextureFormat::RG32F);
    // tex.setWrapMode(GLTextureWrapMode::ClampToEdge);
    // tex.setMinMagFilters(GLTextureFilter::Linear, GLTextureFilter::Linear);
    // FrameBufferObject fbo(width, width);
    // fbo.addColorAttachment(tex);
    // fbo.bind();
    // auto shader = ShaderManager::instance()->load({
    //     {GLShader::Vertex, path::shaders_prefix + "simple2D.vert"},
    //     {GLShader::Fragment, path::shaders_prefix + "IBL/BRDF.frag"},
    // });
    // shader->bind();
    // Geometry::quad()->draw();
    // shader->release();

    // // auto gl = QOpenGLContext::currentContext()->functions();
    // // 三通道保存，因为读取是三通道读取
    // // LImage img(width, width, 3, PixelType::Float32);
    // // gl->glReadPixels(0, 0, width, width, GL_RGB, GL_FLOAT, img.data());
    // // img.saveAsHDR((path::res_prefix + "BRDF.hdr").toLocal8Bit());

    // fbo.release();
    // return tex;
}
