#version 460 core

#include "common/phong.glsl"

out vec4 frag_color;
in VS_OUT {
    vec3 normal;
    vec2 uv;
    vec3 color;
    vec3 view_direction;
    vec3 world_position;
    vec3 T;
    vec3 B;
} fs_in;

struct PBRMaterial
{
    sampler2D albedo_map;
    sampler2D normal_map;
    sampler2D metallic_map;
    sampler2D roughness_map;
    sampler2D ao_map;
};

uniform DirectionalLight directional_light[6];
uniform int directional_light_num;
uniform PointLight point_light[10];
uniform int point_light_num;
uniform SpotLight spot_light[4];
uniform int spot_light_num;
uniform PBRMaterial material;


const float PI = 3.141592653589793;
const float epsilon = 0.0000001;

float NDF_GGX(vec3 normal, vec3 halfway, float roughness)
{
    float a = roughness * roughness;
    float a_squared = a * a;
    float n_dot_h = max(dot(normal, halfway), 0.0);
    float denominator = (n_dot_h * n_dot_h * (a_squared - 1.0) + 1.0) * PI;
    denominator = max(denominator, epsilon);
    return a_squared / denominator;
}

float G_SchlickGGX(float n_dot_v, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float denominator = n_dot_v * (1.0 - k) + k;
    denominator = max(denominator, epsilon);
    return n_dot_v / denominator;
}

float G_Smith(vec3 normal, vec3 view_direction, vec3 light_direction, float roughness)
{
    float n_dot_v = max(dot(normal, view_direction), 0.0);
    float n_dot_l = max(dot(normal, light_direction), 0.0);
    float g1 = G_SchlickGGX(n_dot_v, roughness);
    float g2 = G_SchlickGGX(n_dot_l, roughness);
    return g1 * g2;
}

vec3 fresnel(float cos_theta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cos_theta, 5.0);
}


vec3 calcPointLight(const in PointLight light, const in PBRMaterial material, vec3 normal, vec3 view_direction, vec3 world_position, vec3 albedo, float metallic, float roughness, vec3 F0)
{
    vec3 light_direction = normalize(light.position - world_position);
    vec3 halfway = normalize(light_direction + view_direction);
    float n_dot_l = max(dot(normal, light_direction), 0.0);
    float n_dot_v = max(dot(normal, view_direction), 0.0);
    float distance_to_light = length(light.position - world_position);
    float attenuation = 1.0 / (distance_to_light * distance_to_light);
    vec3 irradiance = light.color * attenuation * n_dot_l;
    float NDF = NDF_GGX(normal, halfway, roughness);
    float G = G_Smith(normal, view_direction, light_direction, roughness);
    vec3 F = fresnel(max(dot(halfway, view_direction), 0.0), F0);
    vec3 ks = F;
    vec3 kd = (1.0 - ks) * (1.0 - metallic);
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * n_dot_v * n_dot_l + epsilon;
    vec3 specularBRDF = numerator / denominator;
    vec3 result = (kd * albedo / PI + specularBRDF) * irradiance;
    return result;
}

void main() {
    mat3 TBN = mat3(normalize(fs_in.T), normalize(fs_in.B), normalize(fs_in.normal));
    vec3 albedo = texture(material.albedo_map, fs_in.uv).rgb;
    float metallic = texture(material.metallic_map, fs_in.uv).g;
    float roughness = texture(material.roughness_map, fs_in.uv).b;
    float ao = texture(material.ao_map, fs_in.uv).r;
    vec3 normal = texture(material.normal_map, fs_in.uv).rgb;
    normal = normal * 2.0 - 1.0;
    normal = normalize(TBN * normal);
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 color = vec3(0.0);
    for (int i = 0; i < point_light_num; ++i) {
        color += calcPointLight(point_light[i], material, normal, fs_in.view_direction, fs_in.world_position, albedo, metallic, roughness, F0);
    }
    vec3 ambient = albedo * (ao + (1.0 - ao) * 0.003);
    color += ambient;
    
    frag_color = vec4(color, 1.0);
}