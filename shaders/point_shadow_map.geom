#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

out GS_OUT {
    float distance_to_light;
} gs_out;

uniform int light_index;

struct PointLightData {
    mat4 light_matrices[6];
    vec3 light_position;
    float far_plane;
};
layout(std430, binding = 3) buffer PointLightMatrices {
    PointLightData light_data[];
};

void main()
{
    float far_plane = light_data[light_index].far_plane;
    vec3 light_position = light_data[light_index].light_position;
    for(int face = 0; face < 6; ++face) {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i) {
            gs_out.distance_to_light = length(light_position - gl_in[i].gl_Position.xyz) / far_plane;
            gl_Position = light_data[light_index].light_matrices[face] * gl_in[i].gl_Position;
            EmitVertex();
        }    
        EndPrimitive();
    }
}