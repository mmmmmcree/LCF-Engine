#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VS_OUT {
    vec3 normal;
    vec3 light_direction;
    float distance_to_light;
} vs_out;

uniform mat4 model;
uniform int light_index;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

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
    vec4 world_position = model * vec4(position, 1.0);
    gl_Position = projection * view * world_position;
    vs_out.normal = normalize(normal);
    vs_out.light_direction = world_position.xyz - light_data[light_index].light_position;
    vs_out.distance_to_light = length(vs_out.light_direction) / (light_data[light_index].far_plane);
    vs_out.light_direction = normalize(vs_out.light_direction);
}