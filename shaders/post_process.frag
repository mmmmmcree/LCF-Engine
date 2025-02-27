#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
} fs_in;

uniform sampler2D channel0;
uniform sampler3D channel1; // color grading LUT
uniform bool hdr_enabled;
uniform bool gamma_correction_enabled;
uniform bool color_grading_enabled;
uniform int color_grading_lut_size;
uniform float exposure;

vec3 tone_mapping_reinhard(vec3 hdr_color) {
	return hdr_color / (hdr_color + vec3(1.0));
}

vec3 tone_mapping_exposure(vec3 hdr_color) {
	return (vec3(1.0) - exp(-hdr_color * exposure));
}

vec3 tone_mapping_aces(vec3 hdr_color) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((hdr_color * (a * hdr_color + b)) / (hdr_color * (c * hdr_color + d) + e), 0.0, 1.0);
}

vec3 gamma_correction(vec3 color) {
    const float gamma = 2.2;
    return pow(color, vec3(1.0 / gamma));
}

vec3 color_grading(sampler3D lut, vec3 color, int lut_size) {
    float scale = (lut_size - 1.0) / lut_size;
    float offset = 1.0 / (2.0 * lut_size);
    return texture(lut, color * scale + offset).rgb;
}

void main() {
    vec3 color = texture(channel0, fs_in.uv).rgb;
    color = mix(color, tone_mapping_exposure(color), hdr_enabled);
    color = mix(color, gamma_correction(color), gamma_correction_enabled);
    color = mix(color, color_grading(channel1, color, color_grading_lut_size), color_grading_enabled);
    frag_color = vec4(color, 1.0);
}