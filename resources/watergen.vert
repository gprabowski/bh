#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 light_pos;
    vec4 light_color;
    vec4 cam_pos;
};

uniform mat4 model;

out vec4 color;
out vec4 frag_pos;

void main() {
    gl_Position = vec4(pos, 1.0);
    frag_pos = vec4(pos, 1.0);
}
