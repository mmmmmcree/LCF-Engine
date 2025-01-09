struct DirectionalLight
{
    vec3 color;
    vec3 direction;
    float intensity;
    int index;
    sampler2D shadow_map;
    bool cast_shadow;
};

struct PointLight
{
    vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    float intensity;
    int index;
    float far_plane;
    samplerCube shadow_map;
    bool cast_shadow;
};

struct SpotLight
{
    vec3 color;
    vec3 position;
    vec3 direction;
    float cos_inner;
    float cos_outer;
    float intensity;
};