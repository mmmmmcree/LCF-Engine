#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 color;

out VS_OUT {
    vec3 normal;
    vec2 uv;
    vec3 color;
    vec3 view_direction;
    vec3 world_position;
} vs_out;

uniform mat4 model;
uniform mat3 normal_matrix;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

void main()
{
    vec4 world_position = model * vec4(position, 1.0);
    gl_Position = projection * view * world_position;
    vs_out.normal = normalize(normal_matrix * normal);
    vs_out.uv = uv;
    vs_out.color = color;
    vs_out.view_direction = normalize(world_position.xyz - camera_position);
    vs_out.world_position = world_position.xyz;
}