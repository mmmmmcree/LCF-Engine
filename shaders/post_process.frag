#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
} fs_in;

uniform sampler2D channel0;
uniform bool hdr_enabled;
uniform bool gamma_correction_enabled;
// uniform float exposure;

vec3 tone_mapping_reinhard(vec3 hdr_color){
	return hdr_color / (hdr_color + vec3(1.0));
}

vec3 tone_mapping_exposure(vec3 hdr_color){
    const float exposure = 1.0; // uniform
	return (vec3(1.0) - exp(-hdr_color * exposure));
}

vec3 gamma_correction(vec3 color) {
    const float gamma = 2.2;
    return pow(color, vec3(1.0 / gamma));
}

void main() {
    vec3 color = texture(channel0, fs_in.uv).rgb;
    float hdr = float(hdr_enabled);
    color = mix(color, tone_mapping_exposure(color), hdr);
    float gamma = float(gamma_correction_enabled);
    color = mix(color, gamma_correction(color), gamma);
    frag_color = vec4(color, 1.0);
}