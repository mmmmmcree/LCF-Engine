#include "Material.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "TextureDispatcher.h"


void lcf::Material::bind()
{
    // int loc = m_start_location = start_location;
    // for (auto &texture : m_textures) {
    //     texture.bind(loc++);
    // }
    for (const auto &[name, texture] : m_texture_map) {
        TextureDispatcher::instance()->setTextureByName(name, texture);
    }
}

// void lcf::Material::release()
// {
//     int loc = m_start_location;
//     for (auto &texture : m_textures) {
//         texture.release(loc++);
//     }
// }


lcf::MaterialType lcf::Material::type() const
{
    return MaterialType::None;
}

const lcf::UniformList &lcf::Material::asUniformList() const
{
    return m_uniforms;
}

const std::string &lcf::Material::fromTextureTypeToUniformName(TextureType type) const
{
    static std::vector<std::string> uniform_names = {
        "none",
        "material.diffuse_map",
        "material.specular_map",
        "material.ambient_map",
        "material.emissive_map",
        "material.height_map",
        "material.normal_map",
        "material.shininess_map",
        "material.opacity_map",
        "material.displacement_map",
        "material.lightmap_map",
        "material.reflection_map",
        "material.albedo_map",
        "material.normal_camera_map",
        "material.emission_color_map",
        "material.metalness_map",
        "material.roughness_map",
        "material.ao_map",
        "unknown",
        "material.sheen_map",
        "material.clear_coat_map",
        "material.transmission_map",
        "material.maya_base_color_map",
        "material.maya_specular_map",
        "material.maya_specular_color_map",
        "material.maya_specular_roughness_map",
        "material.anisotropy_map",
        "material.gltf_metallic_roughness_map",
        "ibl_material.irradiance_map",
        "ibl_material.prefilter_map",
        "ibl_material.brdf_map",
        "channel0",
        "channel1",
        "channel2",
        "channel3",
        "channel4",
        "channel5",
        "channel6",
        "channel7",
        "channel8",
        "channel9",
        "channel10",
        "channel11",
        "channel12",
        "channel13",
        "channel14",
        "channel15"
    };
    return uniform_names[static_cast<int>(type)];
}
