#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec3 normal;
    vec3 light_direction;
    float distance_to_light;
} fs_in;

uniform samplerCube channel0;

struct PointLightData {
    mat4 light_matrices[6];
    vec3 light_position;
    float far_plane;
};
layout(std430, binding = 3) buffer PointLightMatrices {
    PointLightData light_data[];
};

float calcShadow(samplerCube shadow_map, vec3 uvw, float current_depth, float bias)
{
    float closest_depth = texture(shadow_map, uvw).r;
    float shadow = step(closest_depth, current_depth - bias);
    return shadow;
}

void main() {
    vec4 color = vec4(1.0);
    float shadow = calcShadow(channel0, fs_in.light_direction, fs_in.distance_to_light, 0.0005);
    frag_color = color * (1.0 - shadow); 
}