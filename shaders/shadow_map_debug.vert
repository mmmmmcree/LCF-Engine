#version 460 core

layout (location = 0) in vec3 position;

out VS_OUT {
    vec4 light_space_coord;
} vs_out;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

layout(std430, binding = 2) buffer LightMatrices {
    mat4 light_matrices[];
};

void main()
{
    vec4 world_position = model * vec4(position, 1.0);
    gl_Position = projection * view * world_position;
    vs_out.light_space_coord = light_matrices[0] * world_position;
}