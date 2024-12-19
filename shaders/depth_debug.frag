#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
} fs_in;

uniform sampler2D channel0;

void main() {
    float depth = texture(channel0, fs_in.uv).r;
    frag_color = vec4(vec3(depth), 1.0);
}