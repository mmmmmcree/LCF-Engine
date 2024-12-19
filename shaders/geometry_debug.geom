#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.4;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

void generateLine(int index, const in mat4 PV) {
    const vec4 delta = vec4(0.02, vec3(0.0));
    gl_Position = PV * (gl_in[index].gl_Position - delta);
    EmitVertex();
    gl_Position = PV * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0));
    EmitVertex();
    gl_Position = PV * (gl_in[index].gl_Position + delta);
    EmitVertex();
    EndPrimitive();
}

void main() {

    mat4 PV = projection * view;
    gl_Position = PV * gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = PV * gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = PV * gl_in[2].gl_Position;
    EmitVertex();
    EndPrimitive();
    generateLine(0, PV);
    generateLine(1, PV);
    generateLine(2, PV);
}