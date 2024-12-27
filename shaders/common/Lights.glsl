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