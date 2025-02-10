#version 460 core

#include "PBR.glsl"

out vec4 frag_color;

in VS_OUT {
    vec3 normal;
    vec2 uv;
    vec3 color;
    vec3 view_direction;
    vec3 world_position;
    vec3 T;
    vec3 B;
} fs_in;

uniform DirectionalLight directional_light[6];
uniform int directional_light_count;
uniform PointLight point_light[10];
uniform int point_light_count;
uniform SpotLight spot_light[4];
uniform int spot_light_count;

uniform PBRMaterial material;
uniform IBLMaterial ibl_material;

void main() {
    vec3 view_direction = fs_in.view_direction;
    mat3 TBN = mat3(normalize(fs_in.T), normalize(fs_in.B), normalize(fs_in.normal));
    vec3 albedo = texture(material.albedo_map, fs_in.uv).rgb;
    float metallic = texture(material.metallic_map, fs_in.uv).g;
    float roughness = texture(material.roughness_map, fs_in.uv).b;
    float ao = texture(material.ao_map, fs_in.uv).r;
    vec3 normal = texture(material.normal_map, fs_in.uv).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 color = vec3(0.0);
    for (int i = 0; i < directional_light_count; ++i) {
        vec3 directional_color = calcDirectionalLight(directional_light[i], normal, view_direction, albedo, metallic, roughness, F0);
        color += directional_color;
    }
    for (int i = 0; i < point_light_count; ++i) {
        vec3 point_color = calcPointLight(point_light[i], normal, view_direction, fs_in.world_position, albedo, metallic, roughness, F0);
        color += point_color;
    }
    vec3 ambient = calc_ibl_ambient(ibl_material, normal, view_direction, albedo, F0, metallic, roughness, ao);
    vec3 emissive_color = texture(material.emissive_map, fs_in.uv).rgb;
    color += ambient + emissive_color;
    frag_color = vec4(color, 1.0);
}