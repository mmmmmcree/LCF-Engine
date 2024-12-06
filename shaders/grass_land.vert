#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VS_OUT {
    vec2 uv;
    vec2 color_uv;
} vs_out;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

void main() {
    vec4 world_position = model * vec4(position, 1.0);
    gl_Position = projection * view * model * vec4(position, 1.0);
    vs_out.uv = uv;
    vs_out.color_uv = world_position.xz;
}