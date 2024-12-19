layout(location = 5) in vec4 bone_ids; 
layout(location = 6) in vec4 bone_weights;

const int MAX_BONES = 150;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bone_matrices[MAX_BONES];

vec3 boneTransformedPosition(vec3 position) {
    ivec4 bone_ids_int = ivec4(bone_ids);
    vec4 final_position = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++) {
        if (bone_ids_int[i] == -1) { continue; }
        if (bone_ids_int[i] >= MAX_BONES) {
            final_position = vec4(position, 1.0f);
            break;
        }
        final_position += bone_matrices[bone_ids_int[i]] * vec4(position, 1.0f) * bone_weights[i];
    }
    return vec3(final_position.xyz);
}