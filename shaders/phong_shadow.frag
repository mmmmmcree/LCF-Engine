#version 460 core

#include "phong.glsl"
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
uniform int directional_light_count;
uniform PointLight point_light[10];
uniform int point_light_count;
uniform SpotLight spot_light[4];
uniform int spot_light_count;
uniform PhongMaterial material;

const float pcf_radius = 0.0018;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
    mat4 projection_view;
};

// vec3 f(vec3 world_position)
// {
//     vec3 color = vec3(0.0);
//     float near = 0.1;
//     float far = 1000.0;
//     int layer_count = 5;
//     vec3 view_position = (view * vec4(world_position, 1.0)).xyz;
//     float depth = -view_position.z;
//     int layer = int(layer_count * (log(depth / near) / log(far / near)));
//     layer = clamp(layer, 0, layer_count);
//     switch (layer) {
//         case 0 : color = vec3(1.0, 0.0, 0.0); break;
//         case 1 : color = vec3(0.0, 1.0, 0.0); break;
//         case 2 : color = vec3(0.0, 0.0, 1.0); break;
//         case 3 : color = vec3(0.0, 1.0, 1.0); break;
//         case 4 : color = vec3(1.0, 1.0, 0.0); break;
//         case 5 : color = vec3(0.0, 0.0, 0.0); break;
//     }
//     return color;
// }

void main() {
    mat3 TBN = mat3(normalize(fs_in.T), normalize(fs_in.B), normalize(fs_in.normal));
    vec3 object_color = texture(material.diffuse_map, fs_in.uv).rgb;
    float specular_mask = texture(material.specular_map, fs_in.uv).r;
    vec3 normal = texture(material.normal_map, fs_in.uv).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    vec3 color = vec3(0.0);
    for (int i = 0; i < directional_light_count; i++) {
        vec3 directional_color = calcDirectionalLight(directional_light[i], material, normal, fs_in.view_direction, object_color, specular_mask);
        if (directional_light[i].cast_shadow) {
            float shadow = calcDirectionalLightShadow(directional_light[i], normal, fs_in.world_position, pcf_radius);
            directional_color *= (1.0 - shadow);
        }
        color += directional_color;
    }
    for (int i = 0; i < point_light_count; i++) {
        vec3 point_color = calcPointLight(point_light[i], material, normal, fs_in.view_direction, fs_in.world_position, object_color, specular_mask);
        if (point_light[i].cast_shadow) {
            float shadow = calcPointLightShadow(point_light[i], normal, fs_in.world_position, pcf_radius);
            point_color *= (1.0 - shadow);
        }
        color += point_color;
    }
    for (int i = 0; i < spot_light_count; i++) {
        color += calcSpotLight(spot_light[i], material, normal, fs_in.view_direction, fs_in.world_position, object_color, specular_mask);
    }
    vec3 ambient_color = vec3(0.08) * object_color;
    color += ambient_color;
    frag_color = vec4(color, 1.0);
}