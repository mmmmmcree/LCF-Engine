#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform int light_index;

layout(std430, binding = 2) buffer LightMatrices {
    mat4 light_matrices[];
};

void main()
{
    gl_Position = light_matrices[light_index] * model * vec4(position, 1.0);
}