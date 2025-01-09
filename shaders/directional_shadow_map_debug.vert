#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VS_OUT {
    vec3 normal;
    vec3 world_position;
} vs_out;

uniform mat4 model;
uniform int light_index;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

void main()
{
    vec4 world_position = model * vec4(position, 1.0);
    gl_Position = projection * view * world_position;
    vs_out.normal = normalize(normal);
    vs_out.world_position = world_position.xyz;
}