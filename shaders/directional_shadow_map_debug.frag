#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec4 light_space_coord;
    vec3 normal;
    vec3 light_direction;
} fs_in;


uniform sampler2D channel0;

// float bias = 0.001; //uniform
const int samples_num = 32;
const float PI = 3.141592653589793;
const float PI2 = 6.283185307179586;
const float tightness = 0.8; // uniform
const float pcf_radius = 0.0018; // uniform

float calcBias(vec3 normal, vec3 light_direction) {
    const float bias = 0.0005;
    vec3 normal_n = normalize(normal);
    vec3 light_direction_n = normalize(light_direction);
    return max(bias * (1.0 - dot(normal_n, -light_direction_n)), bias);
}

float calcShadow(sampler2D shadow_map, const in vec4 light_space_coord, float bias)
{
    vec3 proj_coord = light_space_coord.xyz / light_space_coord.w;
    proj_coord = proj_coord * 0.5 + 0.5;
    float closest_depth = texture(shadow_map, proj_coord.xy).r;
    float current_depth = proj_coord.z;
    float shadow = step(closest_depth, current_depth - bias);
    return shadow;
}

float rand_2to1(vec2 uv)
{ 
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot(uv.xy, vec2( a,b )), sn = mod(dt, PI);
	return fract(sin(sn) * c);
}

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

float pcf(const sampler2D depth_map, const in vec4 light_space_coord, float bias)
{
    vec3 proj_coord = light_space_coord.xyz / light_space_coord.w;
    proj_coord = proj_coord * 0.5 + 0.5;
    vec2 uv = proj_coord.xy;
    float current_depth = proj_coord.z;
    current_depth -= bias;
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(depth_map, 0);
    float du = texel_size.x;
    float dv = texel_size.y;
    shadow += step(texture(depth_map, uv + vec2(-du, -dv)).r, current_depth);
    shadow += step(texture(depth_map, uv + vec2(0.0, -dv)).r, current_depth);
    shadow += step(texture(depth_map, uv + vec2(du, -dv)).r, current_depth);
    shadow += step(texture(depth_map, uv + vec2(-du, 0.0)).r, current_depth);
    shadow += step(texture(depth_map, uv + vec2(du, 0.0)).r, current_depth);
    shadow += step(texture(depth_map, uv + vec2(-du, dv)).r, current_depth);
    shadow += step(texture(depth_map, uv + vec2(0.0, dv)).r, current_depth);
    shadow += step(texture(depth_map, uv + vec2(du, dv)).r, current_depth);
    shadow += step(texture(depth_map, uv).r, current_depth);
    return shadow / 9.0;
}

float poissonPCF(const sampler2D depth_map, const in vec4 light_space_coord, float bias)
{
    vec3 proj_coord = light_space_coord.xyz / light_space_coord.w;
    proj_coord = proj_coord * 0.5 + 0.5;
    vec2 uv = proj_coord.xy;
    float current_depth = proj_coord.z;
    current_depth -= bias;
    poissonDiskSamples(uv);
    float shadow = 0.0;
    for(int i = 0; i < samples_num; i++){
        float closest_depth = texture(depth_map, uv + disk[i] * pcf_radius).r;
        shadow += step(closest_depth, current_depth);
    }
    return shadow / float(samples_num);
}

void main() {
    vec4 color = vec4(1.0);
    float bias = calcBias(fs_in.normal, fs_in.light_direction);
    float shadow = poissonPCF(channel0, fs_in.light_space_coord, bias);
    frag_color = color * (1.0 - shadow);
}