#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec3 normal;
    vec3 light_direction;
    float distance_to_light;
} fs_in;

uniform samplerCube channel0;

struct PointLightData {
    mat4 light_matrices[6];
    vec3 light_position;
    float far_plane;
};
layout(std430, binding = 3) buffer PointLightMatrices {
    PointLightData light_data[];
};

const int samples_num = 32;
const float PI = 3.141592653589793;
const float PI2 = 6.283185307179586;
const float tightness = 0.8; // uniform
const float pcf_radius = 0.0018; // uniform

float rand_2to1(vec2 uv)
{ 
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot(uv.xy, vec2( a,b )), sn = mod(dt, PI);
	return fract(sin(sn) * c);
}

vec3 disk[samples_num];
void poissonDiskSamples(vec2 random_seed){
	float angle = rand_2to1(random_seed) * PI2;
	float radius = 1.0 / float(samples_num);
	const float da = 3.883222077450933;
	float dr = radius;
	for(int i = 0; i < samples_num; i++){
        float sin_angle = sin(angle);
        float cos_angle = cos(angle);
		disk[i] = vec3(sin_angle * cos_angle, cos_angle, sin_angle * sin_angle) * radius;
		radius += dr;
		angle += da;
	}
}

float calcBias(vec3 normal, vec3 light_direction) {
    const float bias = 0.0005;
    vec3 normal_n = normalize(normal);
    vec3 light_direction_n = normalize(light_direction);
    return max(bias * (1.0 - dot(normal_n, -light_direction_n)), bias);
}

float calcShadow(samplerCube shadow_map, vec3 uvw, float current_depth, float bias)
{
    float closest_depth = texture(shadow_map, uvw).r;
    float shadow = step(closest_depth, current_depth - bias);
    return shadow;
}

float pcf(samplerCube shadow_map, vec3 uvw, float current_depth, float bias) 
{
    float shadow = 0.0;
    poissonDiskSamples(uvw.xy);
    for(int i = 0; i < samples_num; i++) {
        vec3 sample_uvw = uvw + disk[i] * pcf_radius;
        float sample_depth = texture(shadow_map, sample_uvw).r;
        shadow += step(sample_depth, current_depth - bias);
    }
    return shadow / float(samples_num);
}

void main() {
    vec4 color = vec4(1.0);
    float bias = calcBias(fs_in.normal, fs_in.light_direction);
    float shadow = pcf(channel0, fs_in.light_direction, fs_in.distance_to_light, bias);
    frag_color = color * (1.0 - shadow); 
}