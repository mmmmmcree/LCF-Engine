#include "Lights.glsl"

#define PI 3.141592653589793
#define epsilon 0.0000001

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

vec3 fresnel_with_roughness(float cos_theta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cos_theta, 5.0);
}   

vec3 calcFactor(vec3 light_direction, vec3 normal, vec3 view_direction, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    light_direction = -light_direction;
    vec3 halfway = normalize(light_direction + view_direction);
    float n_dot_l = max(dot(normal, light_direction), 0.0);
    float n_dot_v = max(dot(normal, view_direction), 0.0);
    float NDF = NDF_GGX(normal, halfway, roughness);
    float G = G_Smith(normal, view_direction, light_direction, roughness);
    vec3 F = fresnel(max(dot(halfway, view_direction), 0.0), F0);
    vec3 ks = F;
    vec3 kd = (1.0 - ks) * (1.0 - metallic);
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * n_dot_v * n_dot_l + epsilon;
    vec3 specularBRDF = numerator / denominator;
    vec3 diffuseBRDF = kd * albedo / PI;
    return (diffuseBRDF + specularBRDF) * n_dot_l;
}

vec3 calcPointLight(const in PointLight light, vec3 normal, vec3 view_direction, vec3 world_position, vec3 albedo, float metallic, float roughness, vec3 F0)
{
    vec3 light_direction = normalize(world_position - light.position);
    float distance_to_light = length(world_position - light.position);
    float attenuation = 1.0 / (distance_to_light * distance_to_light);
    vec3 factor = calcFactor(light_direction, normal, view_direction, albedo, roughness, metallic, F0);
    return factor * light.color * attenuation;
}

vec3 calcDirectionalLight(const in DirectionalLight light, vec3 normal, vec3 view_direction, vec3 albedo, float metallic, float roughness, vec3 F0)
{
    vec3 light_direction = normalize(light.direction);
    vec3 factor = calcFactor(light_direction, normal, view_direction, albedo, roughness, metallic, F0);
    return factor * light.color;
}

struct PBRMaterial
{
    sampler2D albedo_map;
    sampler2D normal_map;
    sampler2D metallic_map;
    sampler2D roughness_map;
    sampler2D ao_map;
    sampler2D emissive_map;
};

struct IBLMaterial
{
    samplerCube irradiance_map;
    samplerCube prefilter_map;
    sampler2D brdf_map;
};

vec3 calc_ibl_diffuse(vec3 irradiance, vec3 albedo, vec3 normal, vec3 view_direction, vec3 F0, float roughness) {
    vec3 KS = fresnel_with_roughness(max(dot(normal, view_direction), 0.0), F0, roughness);
    vec3 KD = 1.0 - KS;
    vec3 diffuse = KD * irradiance * albedo;
    return diffuse;
}

vec3 calc_ibl_ambient(IBLMaterial material, vec3 normal, vec3 view_direction, vec3 albedo, vec3 F0, float metallic, float roughness, float ao)
{
    float n_dot_v = max(dot(normal, view_direction), 0.0);
    vec3 F = fresnel_with_roughness(n_dot_v, F0, roughness);
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;
    vec3 irradiance = texture(material.irradiance_map, normal).rgb;
    vec3 diffuse = irradiance * albedo;
    const float max_reflection_lod = 4.0;
    vec3 reflect_direction = reflect(view_direction, normal);
    vec3 prefiltered_color = textureLod(material.prefilter_map, reflect_direction, roughness * max_reflection_lod).rgb;
    vec2 brdf = texture(material.brdf_map, vec2(n_dot_v, roughness)).rg;
    vec3 specular = prefiltered_color * (F * brdf.x + vec3(brdf.y));
    vec3 ambient = (kD * diffuse + specular) * ao;
    return ambient;
}