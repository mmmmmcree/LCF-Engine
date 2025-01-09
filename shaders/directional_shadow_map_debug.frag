#version 460 core

#include "shadow_common.glsl"
#include "directional_light_shadow.glsl"

out vec4 frag_color;

in VS_OUT {
    vec3 normal;
    vec3 world_position;
} fs_in;

uniform sampler2D channel0;
uniform DirectionalLight directional_light[1];
const float pcf_radius = 0.0018; // uniform

void main() {
    vec4 color = vec4(1.0);
    float shadow = calcDirectionalLightShadow(directional_light[0], fs_in.normal, fs_in.world_position, pcf_radius);
    frag_color = color * (1.0 - shadow);
}