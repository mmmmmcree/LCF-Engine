#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VS_OUT {
    vec2 uv;
} vs_out;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    vs_out.uv = uv;
}