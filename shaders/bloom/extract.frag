#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
} fs_in;

uniform sampler2D channel0;
uniform float threshold;

float calculateAttenuation(float brightness, float threshold, float k) {
    float excess = max(brightness - threshold, 0.0);
    // return exp(-k * excess);            // 指数衰减
    // return 1.0 / (1.0 + k * excess);    // 反比例函数
    return 1.0 / (1.0 + pow(k * excess, 2.0)); // 幂函数（n=2）
}

void main() {
    vec3 color = texture(channel0, fs_in.uv).rgb;
    color = max(vec3(0.0), color);
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    float brighter = step(threshold, brightness);
    color = mix(vec3(0.0), color, brighter);
    frag_color = vec4(color, calculateAttenuation(brightness, threshold, 0.5));
}