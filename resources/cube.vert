#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 bh_data;
    vec4 exec_data;
};

uniform mat4 model;

out vec2 frag_pos;

void main() {
    frag_pos = pos.xy;
    gl_Position = vec4(pos, 1.0);
}
