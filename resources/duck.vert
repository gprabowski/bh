#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;

layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 light_pos;
    vec4 light_color;
    vec4 cam_pos;
};

uniform mat4 model;

out vec3 frag_pos;
out vec3 normal;
out vec2 ftex;

void main() {
    gl_Position = proj * view * model * vec4(pos, 1.0);
    frag_pos = vec3(model * vec4(pos, 1.0));
    normal = transpose(inverse(mat3(model))) * norm;
    ftex = tex;
}
