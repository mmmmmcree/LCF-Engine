#version 460 core

#include "common/animation.glsl"

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform int light_index;

layout(std430, binding = 2) buffer LightMatrices {
    mat4 light_matrices[];
};

void main()
{
    vec3 final_position = boneTransformedPosition(position);
    gl_Position = light_matrices[light_index] * model * vec4(final_position, 1.0);
}