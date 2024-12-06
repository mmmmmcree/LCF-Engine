#version 460 core

layout(location = 0) in vec3 position;

out VS_OUT {
    vec3 uvw;
} vs_out;

layout(std140, binding = 0) uniform Matrices {
    mat4 view;
    mat4 projection;
    vec3 camera_position;
};

void main() {
    mat4 skybox_view = mat4(mat3(view));
    gl_Position = projection * skybox_view * vec4(position, 1.0);
    gl_Position = gl_Position.xyww;
    vs_out.uvw = position;
}