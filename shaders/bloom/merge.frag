#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
} fs_in;

uniform sampler2D channel0; // original image
uniform sampler2D channel1; // bloom image
uniform float bloom_intensity;

void main() {
    vec3 color = texture(channel0, fs_in.uv).rgb;
    vec3 bloom = texture(channel1, fs_in.uv).rgb;
    frag_color = vec4(color + bloom * bloom_intensity, 1.0);
}