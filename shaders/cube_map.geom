#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

in VS_OUT {
    vec3 uvw;
} gs_in[];

out GS_OUT {
    vec3 uvw;
} gs_out;

const mat4 projection = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, -1.002, -1.0, 0.0, 0.0, -0.2002, 0.0);
const mat4 view[6] = mat4[](
    mat4(0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0),
    mat4(0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0),
    mat4(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0),
    mat4(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0),
    mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0),
    mat4(-1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0)
);

void main()
{
    for(int face = 0; face < 6; ++face) {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i) {
            gl_Position = projection * view[face] * gl_in[i].gl_Position;
            gs_out.uvw = gs_in[i].uvw;
            EmitVertex();
        }    
        EndPrimitive();
    }
}