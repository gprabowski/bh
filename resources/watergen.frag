#version 460

in vec4 frag_pos;

layout (location = 0) out vec4 height;
layout (location = 1) out vec4 normal;

layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 light_pos;
    vec4 light_color;
    vec4 cam_pos;
};

uniform sampler2D height_texture;

void main() {
    height = vec4(sin(1000 * length(frag_pos)), 0.0, 0.0, 1.0);
    normal = vec4(1.0, 1.0, 1.0, 1.0);
}
