#version 460 core

layout (location = 0) in vec3 position;

out VS_OUT {
    vec3 uvw;
} vs_out;

void main() {
    gl_Position = vec4(position, 1.0);
    vs_out.uvw = position;
}
