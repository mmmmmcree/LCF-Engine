#version 460 core

in GS_OUT {
    float distance_to_light;
} gs_out;

void main()
{
    gl_FragDepth = gs_out.distance_to_light;
}