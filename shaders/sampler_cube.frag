#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec3 uvw;
} fs_in;

uniform samplerCube channel0;

void main()
{
    vec3 color = texture(channel0, normalize(fs_in.uvw)).rgb;
    frag_color = vec4(color, 1.0);
}