#version 460 core

in GS_OUT {
    vec3 uvw;
} fs_in;

out vec4 frag_color;

uniform sampler2D channel0;

const float PI = 3.14159265359;

vec2 spherical_uv(vec3 uvw) {
    uvw = normalize(uvw);
    float phi = asin(uvw.y);
    float theta = atan(uvw.z, uvw.x);
    float u = 0.5 + 0.5 * theta / PI;
    float v = phi / PI + 0.5;
    return vec2(u, v);
}

void main() {
    vec3 color = texture(channel0, spherical_uv(fs_in.uvw)).rgb;
    frag_color = vec4(color, 1.0);
}