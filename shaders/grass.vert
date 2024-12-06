#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

out VS_OUT {
    vec2 uv;
    vec2 color_uv;
} vs_out;

uniform mat4 model;
uniform float time;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

layout(std430, binding = 1) buffer InstanceMatrices {
    mat4 instance_offsets[];
};

void main() {
    vec4 world_position = (model * instance_offsets[gl_InstanceID] * vec4(position, 1.0));
    vec3 offset_direction = vec3(1.0, 0.1, 0.0);
    float offset_intensity = 0.8;
    float distance_on_direction = dot(offset_direction, world_position.xyz);
    float t = 0.1 * sin(0.5 * time + distance_on_direction * 0.5) * sin(time);
    vec3 offset = sin(t) * offset_intensity * (1.0 - color.r) * offset_direction;
    world_position.xyz += offset;
    gl_Position = projection * view * world_position;
    vs_out.uv = uv;
    vs_out.color_uv = world_position.xz - offset.xz;
}