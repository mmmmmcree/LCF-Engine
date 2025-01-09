const int samples_num = 32;
#define PI 3.141592653589793
#define PI2 6.283185307179586
// const float tightness = 0.8; // uniform

float rand_2to1(vec2 uv)
{ 
	const highp float a = 12.9898, b = 78.233, c = 43758.5453;
	highp float dt = dot(uv.xy, vec2( a,b )), sn = mod(dt, PI);
	return fract(sin(sn) * c);
}

float calcBias(vec3 normal, vec3 light_direction) {
    const float bias = 0.0005;
    vec3 normal_n = normalize(normal);
    vec3 light_direction_n = normalize(light_direction);
    return max(bias * (1.0 - dot(normal_n, -light_direction_n)), bias);
}