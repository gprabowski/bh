#version 460

in vec4 color;
in vec3 fragtex;
out vec4 frag_color;

uniform sampler2D normal_map;
uniform sampler2D height_map;

layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 light_pos;
    vec4 light_color;
    vec4 cam_pos;
};

void main() {
    frag_color = texture(height_map, fragtex.xy);
}
