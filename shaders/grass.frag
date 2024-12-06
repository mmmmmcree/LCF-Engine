#version 460 core

out vec4 frag_color;

in VS_OUT {
    vec2 uv;
    vec2 color_uv;
} fs_in;

uniform float time;
uniform sampler2D channel0; //  grass
uniform sampler2D channel1; // grass opacity mask
uniform sampler2D channel2; // cloud opacity mask

void main() {
	float alpha =  texture(channel1, fs_in.uv).r;
	if(alpha < 0.1){ discard; }
    vec2 grass_uv = fs_in.color_uv * 0.05;
    vec2 cloud_uv = fs_in.color_uv * 0.04;
    float cloud_speed = 0.02;
    cloud_uv.x += time * cloud_speed;
    vec4 grass_color = texture(channel0, grass_uv) * 1.8;
    float cloud_mask = texture(channel2, cloud_uv).r;
    vec4 cloud_white = grass_color;
    vec4 cloud_black = vec4(vec3(-0.6), 1.0);
    vec4 cloud_color = mix(cloud_black, cloud_white, cloud_mask);
    frag_color = mix(grass_color, cloud_color, 0.2);
}