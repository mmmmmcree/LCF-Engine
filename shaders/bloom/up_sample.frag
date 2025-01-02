#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
} fs_in;

uniform sampler2D channel0; //low res
uniform sampler2D channel1; //high res
uniform float bloom_radius;
uniform float bloom_attenuation;
const float PI = 3.141592653589793;
const float PI2 = 6.283185307179586;

float rand_2to1(vec2 uv)
{ 
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot(uv.xy, vec2( a,b )), sn = mod(dt, PI);
	return fract(sin(sn) * c);
}

const int samples_num = 32;
vec2 disk[samples_num];
void poissonDiskSamples(vec2 random_seed){
	float angle = rand_2to1(random_seed) * PI2;
	float radius = 1.0 / float(samples_num);
	const float da = 3.883222077450933;
	float dr = radius;
	for(int i = 0; i < samples_num; i++){
		disk[i] = vec2(cos(angle), sin(angle)) * radius;
		radius += dr;
		angle += da;
	}
}

vec3 poissonSample(sampler2D channel, vec2 uv)
{
    poissonDiskSamples(uv);
    vec3 color = vec3(0.0);
    for(int i = 0; i < samples_num; i++) {
        color += texture(channel, uv + disk[i] * bloom_radius).rgb;
    }
    return color / float(samples_num);
}

void main() {
    vec3 low_res_color = poissonSample(channel0, fs_in.uv);
    vec3 high_res_color = poissonSample(channel1, fs_in.uv);
    vec3 bloom_color = mix(low_res_color, high_res_color, bloom_attenuation);
    frag_color = vec4(bloom_color, 1.0);
}