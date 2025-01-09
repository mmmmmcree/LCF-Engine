#version 460 core

#include "animation.glsl"

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform int light_index;


struct LightData {
    mat4 light_matrix;
    mat4 light_view_matrix;
    vec3 light_direction;
    float padding;
};
layout(std430, binding = 2) buffer LightMatrices {
    LightData light_data[];
};

void main()
{
    vec3 final_position = boneTransformedPosition(position);
    gl_Position = light_data[light_index].light_matrix * model * vec4(final_position, 1.0);
}