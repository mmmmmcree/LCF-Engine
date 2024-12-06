#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
// layout(location = 3) in ivec4 bone_ids; 
layout(location = 4) in vec4 bone_ids; 
layout(location = 5) in vec4 bone_weights;

out VS_OUT {
    vec2 uv;
} vs_out;

const int MAX_BONES = 150;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bone_matrices[MAX_BONES];
uniform mat4 model;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

void main()
{
    ivec4 bone_ids_int = ivec4(bone_ids);
    vec4 final_position = vec4(0.0f);
    for(int i = 0 ; i < 4 ; i++) {
        if (bone_ids_int[i] == -1) { continue; }
        if (bone_ids_int[i] >= MAX_BONES) {
            final_position = vec4(position, 1.0f);
            break;
        }
        final_position += bone_matrices[bone_ids_int[i]] * vec4(position, 1.0f) * bone_weights[i];
    }
    gl_Position = projection * view * model * final_position;
    vs_out.uv = uv;
}
