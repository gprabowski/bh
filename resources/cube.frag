#version 460

in vec3 normal;
in vec3 frag_pos;

out vec4 frag_color;

layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 light_pos;
    vec4 light_color;
    vec4 cam_pos;
};

uniform samplerCube skybox;

void main() {
    frag_color = texture(skybox, normalize(frag_pos));
}
