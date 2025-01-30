#version 460 core

in GS_OUT {
    vec3 uvw;
} fs_in;

out vec4 frag_color;

uniform samplerCube channel0;
const float PI = 3.14159265359;

void main() {
    vec3 normal = normalize(fs_in.uvw);
    vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), normal));
    vec3 up = normalize(cross(normal, right));
    float delta_rad = 0.025;
    float double_pi = 2.0 * PI;
    float pi_over_2 = 0.5 * PI;
    int sample_num = 0;
    vec3 irradiance = vec3(0.0);
    for (float phi = 0.0; phi < double_pi; phi += delta_rad) {
        for (float theta = 0.0; theta < pi_over_2; theta += delta_rad) {
            float sin_theta = sin(theta);
            float cos_theta = cos(theta);
            float sin_phi = sin(phi);
            float cos_phi = cos(phi);
            vec3 local = vec3(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
            vec3 world = local.x * right + local.y * up + local.z * normal;
            vec3 uvw = normalize(world);
            irradiance += texture(channel0, uvw).rgb * cos_theta * sin_theta;
            ++sample_num;
        }
    }
    irradiance = PI * irradiance / float(sample_num);
    frag_color = vec4(irradiance, 1.0);
}