#version 460 core

#include "PBR.glsl"
#include "directional_light_shadow.glsl"
#include "point_light_shadow.glsl"

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
uniform int directional_light_num;
uniform PointLight point_light[10];
uniform int point_light_num;
uniform SpotLight spot_light[4];
uniform int spot_light_num;

uniform PBRMaterial material;

const float pcf_radius = 0.0018;


void main() {
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
    for (int i = 0; i < directional_light_num; ++i) {
        vec3 directional_color = calcDirectionalLight(directional_light[i], normal, fs_in.view_direction, albedo, metallic, roughness, F0);
        if (directional_light[i].cast_shadow) {
            float shadow = calcDirectionalLightShadow(directional_light[i], normal, fs_in.world_position, pcf_radius);
            directional_color *= (1.0 - shadow);
        }
        color += directional_color;
    }
    for (int i = 0; i < point_light_num; ++i) {
        vec3 point_color = calcPointLight(point_light[i], normal, fs_in.view_direction, fs_in.world_position, albedo, metallic, roughness, F0);
        if (point_light[i].cast_shadow) {
            float shadow = calcPointLightShadow(point_light[i], normal, fs_in.world_position, pcf_radius);
            point_color *= (1.0 - shadow);
        }
        color += point_color;
    }
    vec3 ambient = albedo * (ao + (1.0 - ao) * 0.003);
    vec3 emissive_color = texture(material.emissive_map, fs_in.uv).rgb;
    color += ambient + emissive_color;
    frag_color = vec4(color, 1.0);
}