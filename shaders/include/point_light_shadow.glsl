#include "Lights.glsl"
#include "shadow_common.glsl"

vec3 disk_3d[samples_num];
void poissonDiskSamples3D(vec2 random_seed){
	float angle = rand_2to1(random_seed) * PI2;
	float radius = 1.0 / float(samples_num);
	const float da = 3.883222077450933;
	float dr = radius;
	for(int i = 0; i < samples_num; i++){
        float sin_angle = sin(angle);
        float cos_angle = cos(angle);
		disk_3d[i] = vec3(sin_angle * cos_angle, cos_angle, sin_angle * sin_angle) * radius;
		radius += dr;
		angle += da;
	}
}

float pointPoissonPCF(samplerCube shadow_map, vec3 uvw, float current_depth, float bias, float pcf_radius) 
{
    float shadow = 0.0;
    poissonDiskSamples3D(uvw.xy);
    for(int i = 0; i < samples_num; i++) {
        vec3 sample_uvw = uvw + disk_3d[i] * pcf_radius;
        float sample_depth = texture(shadow_map, sample_uvw).r;
        shadow += step(sample_depth, current_depth - bias);
    }
    return shadow / float(samples_num);
}

float calcPointLightShadow(const in PointLight light, vec3 normal, vec3 world_position, float pcf_radius)
{
    vec3 light_direction = normalize(world_position - light.position);
    float distance_to_light = length(world_position - light.position) / light.far_plane;
    float bias = calcBias(normal, light_direction);
    return pointPoissonPCF(light.shadow_map, light_direction, distance_to_light, bias, pcf_radius);
}