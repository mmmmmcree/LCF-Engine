#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
    vec2 color_uv;
} fs_in;

uniform sampler2D channel0;

void main() {
    vec2 land_uv = fs_in.color_uv * 0.05;
    frag_color = texture(channel0, land_uv) * 1.8;
}