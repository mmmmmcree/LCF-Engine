#include "Lights.glsl"
#include "shadow_common.glsl"

vec2 disk_2d[samples_num];
void poissonDiskSamples2D(vec2 random_seed){
	float angle = rand_2to1(random_seed) * PI2;
	float radius = 1.0 / float(samples_num);
	const float da = 3.883222077450933;
	float dr = radius;
	for(int i = 0; i < samples_num; i++){
		disk_2d[i] = vec2(cos(angle), sin(angle)) * radius;
		radius += dr;
		angle += da;
	}
}

float directionalPCF(const sampler2D depth_map, const in vec4 light_space_coord, float bias)
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

float directionalPoissonPCF(const sampler2D depth_map, const in vec4 light_space_coord, float bias, float pcf_radius)
{
    vec3 proj_coord = light_space_coord.xyz / light_space_coord.w;
    proj_coord = proj_coord * 0.5 + 0.5;
    vec2 uv = proj_coord.xy;
    float current_depth = proj_coord.z;
    current_depth -= bias;
    poissonDiskSamples2D(uv);
    float shadow = 0.0;
    for(int i = 0; i < samples_num; i++){
        float closest_depth = texture(depth_map, uv + disk_2d[i] * pcf_radius).r;
        shadow += step(closest_depth, current_depth);
    }
    return shadow / float(samples_num);
}

struct DirectionalLightData {
    mat4 light_matrix;
    mat4 light_view_matrix;
    vec3 light_direction;
    float padding;
};
layout(std430, binding = 2) buffer DirectionalLightDataList {
    DirectionalLightData directional_light_data[];
};

float calcDirectionalLightShadow(const in DirectionalLight light, vec3 normal, vec3 world_position, float pcf_radius)
{
    vec3 light_direction = normalize(light.direction);
    vec4 light_space_coord = directional_light_data[light.index].light_matrix * vec4(world_position, 1.0);
    float bias = calcBias(normal, light_direction);
    float shadow = directionalPoissonPCF(light.shadow_map, light_space_coord, bias, pcf_radius);
    return shadow;
}