#version 460 core

in GS_OUT {
    vec3 world_position;
} fs_in;

uniform int light_index;

struct PointLightData {
    mat4 light_matrices[6];
    vec3 light_position;
    float far_plane;
};
layout(std430, binding = 3) buffer PointLightMatrices {
    PointLightData light_data[];
};

void main()
{
    vec3 light_pos = light_data[light_index].light_position;
    float far_plane = light_data[light_index].far_plane;
    float distance_to_light = length(fs_in.world_position - light_pos);
    distance_to_light = distance_to_light / (1.4142135623730951 * far_plane);
    gl_FragDepth = distance_to_light;
}