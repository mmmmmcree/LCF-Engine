struct DirectionalLight
{
    vec3 color;
    vec3 direction;
    float diffuse_intensity;
    float specular_intensity;
    float ambient_intensity;
};

struct PointLight
{
    vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    float diffuse_intensity;
    float specular_intensity;
    float ambient_intensity;
};

struct SpotLight
{
    vec3 color;
    vec3 position;
    vec3 direction;
    float cos_inner;
    float cos_outer;
    float diffuse_intensity;
    float specular_intensity;
    float ambient_intensity;
};

struct Material
{
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D normal_map;
    float shininess;
};

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

vec3 calcDirectionalLight(DirectionalLight light, Material material, vec3 normal, vec3 view_direction, vec3 object_color, float specular_mask)
{
    vec3 light_direction = normalize(light.direction);
    float diffuse_factor = calcDiffuse(normal, light_direction);
    float specular_factor = calcSpecular(normal, light_direction, view_direction, material.shininess, specular_mask);
    vec3 diffuse_color = object_color * diffuse_factor * light.diffuse_intensity;
    vec3 specular_color = vec3(specular_mask * specular_factor * light.specular_intensity);
    vec3 ambient_color = object_color * light.ambient_intensity;
    vec3 color = (diffuse_color + specular_color + ambient_color) * light.color;
    return color;
}

vec3 calcPointLight(PointLight light, Material material, vec3 normal, vec3 view_direction, vec3 world_position, vec3 object_color, float specular_mask)
{
    vec3 light_direction = normalize(world_position - light.position);
    float distance = length(world_position - light.position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    float diffuse_factor = calcDiffuse(normal, light_direction);
    float specular_factor = calcSpecular(normal, light_direction, view_direction, material.shininess, specular_mask);
    vec3 diffuse_color = object_color * diffuse_factor * light.diffuse_intensity;
    vec3 specular_color = vec3(specular_mask * specular_factor * light.specular_intensity);
    vec3 ambient_color = object_color * light.ambient_intensity;
    vec3 color = (diffuse_color + specular_color + ambient_color) * light.color * attenuation;
    return color;
}

vec3 calcSpotLight(SpotLight light, Material material, vec3 normal, vec3 view_direction, vec3 world_position, vec3 object_color, float specular_mask)
{
    vec3 light_direction = normalize(world_position - light.position);
    vec3 spot_direction = normalize(light.direction);
    float cos_gamma = dot(light_direction, spot_direction);
    float attenuation = clamp((cos_gamma - light.cos_outer) / (light.cos_inner - light.cos_outer), 0.0, 1.0);
    float diffuse_factor = calcDiffuse(normal, light_direction);
    float specular_factor = calcSpecular(normal, light_direction, view_direction, material.shininess, specular_mask);
    vec3 diffuse_color = object_color * diffuse_factor * attenuation * light.diffuse_intensity;
    vec3 specular_color = vec3(specular_mask * specular_factor * attenuation * light.specular_intensity);
    vec3 ambient_color = object_color * light.ambient_intensity;
    vec3 color = (diffuse_color + specular_color + ambient_color) * light.color;
    return color;
}