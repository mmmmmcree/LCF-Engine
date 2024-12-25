#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
} fs_in;

uniform sampler2D channel0;

void main() {
    vec4 color = texture(channel0, fs_in.uv);
    if (color.a < 0.3) { discard; }
    frag_color = color;
    frag_color = vec4(1.0);
}