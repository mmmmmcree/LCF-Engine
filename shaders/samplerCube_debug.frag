#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec3 uvw;
} fs_in;

uniform samplerCube channel0;

void main() {
    frag_color = texture(channel0, fs_in.uvw);
}