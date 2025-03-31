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
uniform vec2 resolution;
uniform bool fxaa_enabled;

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

vec3 fxaa(sampler2D screen_tex, vec2 uv, vec2 resolution)
{
    const float FXAA_REDUCE_MIN = 1.0 / 128.0; //最小对比度阈值，避免处理低对比度区域
    const float FXAA_REDUCE_MUL = 1.0 / 8.0; // 动态阈值系数，根据场景亮度调整灵敏度
    const float FXAA_SPAN_MAX = 8.0; // ：控制边缘方向采样的最大步长，值越大抗锯齿越强，但可能模糊细节
    
    const vec3 luma_weight = vec3(0.299, 0.587, 0.114);

    vec2 res_step = 1.0 / resolution;
    
    vec3 rgb_nw = texture(screen_tex, uv + vec2(-1.0, -1.0) * res_step).rgb;
    vec3 rgb_ne = texture(screen_tex, uv + vec2(1.0, -1.0) * res_step).rgb;
    vec3 rgb_sw = texture(screen_tex, uv + vec2(-1.0, 1.0) * res_step).rgb;
    vec3 rgb_se = texture(screen_tex, uv + vec2(1.0, 1.0) * res_step).rgb;
    vec3 rgb_m  = texture(screen_tex, uv).rgb;

    float luma_nw = dot(rgb_nw, luma_weight);
    float luma_ne = dot(rgb_ne, luma_weight);
    float luma_sw = dot(rgb_sw, luma_weight);
    float luma_se = dot(rgb_se, luma_weight);
    float luma_m  = dot(rgb_m,  luma_weight);
    float luma_min = min(luma_m, min(min(luma_nw, luma_ne), min(luma_sw, luma_se)));
    float luma_max = max(luma_m, max(max(luma_nw, luma_ne), max(luma_sw, luma_se)));
    vec3 color = vec3(0.0);
    bool early_return = (luma_max - luma_min < max(FXAA_REDUCE_MIN, luma_max * FXAA_REDUCE_MUL));
    color = mix(color, rgb_m, early_return);
    vec2 dir = vec2(-((luma_nw + luma_ne) - (luma_sw + luma_se)), ((luma_nw + luma_sw) - (luma_ne + luma_se)));
    float dir_reduce = max((luma_nw + luma_ne + luma_sw + luma_se) * 0.25 * FXAA_REDUCE_MUL, FXAA_REDUCE_MIN);
    float dir_min = 1.0 / (min(abs(dir.x), abs(dir.y)) + dir_reduce);
    dir = clamp(dir * dir_min, vec2(-FXAA_SPAN_MAX), vec2(FXAA_SPAN_MAX)) * res_step;
    vec3 rgb_a = 0.5 * (
        texture(screen_tex, uv + dir * (1.0/3.0 - 0.5)).rgb +
        texture(screen_tex, uv + dir * (2.0/3.0 - 0.5)).rgb
    );
    vec3 rgb_b = rgb_a * 0.5 + 0.25 * (
        texture(screen_tex, uv + dir * -0.5).rgb +
        texture(screen_tex, uv + dir * 0.5).rgb
    );
    float luma_b = dot(rgb_b, luma_weight);
    color = color + mix(mix(rgb_a, rgb_b, luma_b < luma_min || luma_b > luma_max), vec3(0.0), early_return);
    return color;
}

void main() {
    vec3 color = mix(texture(channel0, fs_in.uv).rgb, fxaa(channel0, fs_in.uv, resolution), fxaa_enabled);
    color = mix(color, tone_mapping_exposure(color), hdr_enabled);
    color = mix(color, gamma_correction(color), gamma_correction_enabled);
    color = mix(color, color_grading(channel1, color, color_grading_lut_size), color_grading_enabled);
    frag_color = vec4(color, 1.0);
}