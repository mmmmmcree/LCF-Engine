#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out VS_OUT {
    vec4 light_space_coord;
    vec3 normal;
    vec3 light_direction;
} vs_out;

uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

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
    vec4 world_position = model * vec4(position, 1.0);
    gl_Position = projection * view * world_position;
    vs_out.light_space_coord = light_data[0].light_matrix * world_position;
    vs_out.normal = normalize(normal);
    vs_out.light_direction = normalize(light_data[0].light_direction);
}