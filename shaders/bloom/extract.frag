#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
} fs_in;

uniform sampler2D channel0;
uniform float threshold;

void main() {
    vec3 color = texture(channel0, fs_in.uv).rgb;
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    color = mix(vec3(0.0), color, step(threshold, brightness));
    frag_color = vec4(color, 1.0);
}