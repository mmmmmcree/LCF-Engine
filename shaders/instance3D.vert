#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VS_OUT {
    vec2 uv;
} vs_out;

uniform mat4 model;
// uniform mat4 instance_offsets[196];

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

layout(std430, binding = 1) buffer InstanceMatrices {
    mat4 instance_offsets[];
};

void main() {
    gl_Position = projection * view * model * instance_offsets[gl_InstanceID] * vec4(position, 1.0);
    vs_out.uv = uv;
}