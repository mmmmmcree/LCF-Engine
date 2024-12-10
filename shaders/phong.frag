#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec3 normal;
    vec2 uv;
    vec3 color;
    vec3 view_direction;
    vec3 world_position;
} fs_in;

struct DirectionalLight
{
    vec3 color;
    float diffuse_intensity;
    float specular_intensity;
    float shininess;
    float ambient_intensity;
    vec3 direction;
};

struct PointLight
{
    vec3 color;
    float diffuse_intensity;
    float specular_intensity;
    float shininess;
    float ambient_intensity;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 color;
    float diffuse_intensity;
    float specular_intensity;
    float shininess;
    float ambient_intensity;
    vec3 position;
    vec3 direction;
    float cos_inner;
    float cos_outer;
};

uniform DirectionalLight directional_light[6];
uniform int directional_light_num;
uniform PointLight point_light[10];
uniform int point_light_num;
uniform SpotLight spot_light[4];
uniform int spot_light_num;

uniform sampler2D diffuse_channel;
uniform sampler2D specular_channel;

float calcDiffuse(vec3 normal, vec3 light_direction)
{
    float diffuse = clamp(dot(-light_direction, normal), 0.0, 1.0);
    return diffuse;
}

float calcSpecular(vec3 normal, vec3 light_direction, vec3 view_direction, float shininess, float mask)
{
    float positive_side = step(0.0, dot(normal, -light_direction));
    vec3 reflect_direction = reflect(light_direction, normal);
    float specular = pow(max(dot(reflect_direction, -view_direction), 0.0), shininess);
    return specular * positive_side * mask;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 view_direction, vec3 object_color, float specular_mask)
{
    float diffuse = calcDiffuse(normal, light.direction);
    float specular = calcSpecular(normal, light.direction, view_direction, light.shininess, specular_mask);
    float factor = (light.ambient_intensity + light.diffuse_intensity * diffuse + light.specular_intensity * specular);
    vec3 color = factor * object_color * light.color;
    return color;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 view_direction, vec3 world_position, vec3 object_color, float specular_mask)
{
    vec3 light_direction = normalize(world_position - light.position);
    float distance = length(world_position - light.position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    float diffuse = calcDiffuse(normal, light_direction);
    float specular = calcSpecular(normal, light_direction, view_direction, light.shininess, specular_mask);
    float factor = (light.ambient_intensity + light.diffuse_intensity * diffuse + light.specular_intensity * specular) * attenuation;
    vec3 color = factor * object_color * light.color;
    return color;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 view_direction, vec3 world_position, vec3 object_color, float specular_mask)
{
    vec3 light_direction = normalize(world_position - light.position);
    vec3 spot_direction = normalize(light.direction);
    float cos_gamma = dot(light_direction, spot_direction);
    float attenuation = clamp((cos_gamma - light.cos_outer) / (light.cos_inner - light.cos_outer), 0.0, 1.0);
    float diffuse = calcDiffuse(normal, light_direction);
    float specular = calcSpecular(normal, light_direction, view_direction, light.shininess, specular_mask);
    float factor = light.ambient_intensity + (light.diffuse_intensity * diffuse + light.specular_intensity * specular) * attenuation;
    vec3 color = factor * object_color * light.color;
    return color;
}

void main() {
    vec3 object_color = texture(diffuse_channel, fs_in.uv).rgb;
    float specular_mask = texture(specular_channel, fs_in.uv).r;
    vec3 color = vec3(0.0);
    for (int i = 0; i < directional_light_num; i++) {
        color += calcDirectionalLight(directional_light[i], fs_in.normal, fs_in.view_direction, object_color, specular_mask);
    }
    for (int i = 0; i < point_light_num; i++) {
        color += calcPointLight(point_light[i], fs_in.normal, fs_in.view_direction, fs_in.world_position, object_color, specular_mask);
    }
    for (int i = 0; i < spot_light_num; i++) {
        color += calcSpotLight(spot_light[i], fs_in.normal, fs_in.view_direction, fs_in.world_position, object_color, specular_mask);
    }
    // color += calcDirectionalLight(directional_light, fs_in.normal, fs_in.view_direction, object_color, specular_mask);
    // color += calcPointLight(point_light, fs_in.normal, fs_in.view_direction, fs_in.world_position, object_color, specular_mask);
    // color += calcSpotLight(spot_light, fs_in.normal, fs_in.view_direction, fs_in.world_position, object_color, specular_mask);
    frag_color = vec4(color, 1.0);
}