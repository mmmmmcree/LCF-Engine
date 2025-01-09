#version 460 core

#include "animation.glsl"

layout (location = 0) in vec3 position;

uniform mat4 model;

void main()
{
    vec3 final_position = boneTransformedPosition(position);
    gl_Position = model * vec4(final_position, 1.0);
}