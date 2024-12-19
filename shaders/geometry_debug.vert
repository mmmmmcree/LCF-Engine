#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat3 normal_matrix;

void main() {
    gl_Position = model * vec4(position, 1.0);
    vs_out.normal = normalize(normal_matrix * normal);
}