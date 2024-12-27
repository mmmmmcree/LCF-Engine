#version 460 core

#include "common/phong.glsl"

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
uniform Material material;

void main() {
    mat3 TBN = mat3(normalize(fs_in.T), normalize(fs_in.B), normalize(fs_in.normal));
    vec3 object_color = texture(material.diffuse_map, fs_in.uv).rgb;
    float specular_mask = texture(material.specular_map, fs_in.uv).r;
    vec3 normal = texture(material.normal_map, fs_in.uv).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    vec3 color = vec3(0.0);
    for (int i = 0; i < directional_light_num; i++) {
        color += calcDirectionalLight(directional_light[i], material, normal, fs_in.view_direction, object_color, specular_mask);
    }
    for (int i = 0; i < point_light_num; i++) {
        color += calcPointLight(point_light[i], material, normal, fs_in.view_direction, fs_in.world_position, object_color, specular_mask);
    }
    for (int i = 0; i < spot_light_num; i++) {
        color += calcSpotLight(spot_light[i], material, normal, fs_in.view_direction, fs_in.world_position, object_color, specular_mask);
    }
    frag_color = vec4(color, 1.0);
}