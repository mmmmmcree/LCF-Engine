#version 460 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 uv;

out VS_OUT {
    vec2 uv;
} vs_out;


void main() {
    gl_Position = vec4(position.xy, 0.0, 1.0);
    vs_out.uv = uv;
}