#version 460

in vec3 normal;
in vec3 frag_pos;
in vec2 ftex;

out vec4 frag_color;

layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 light_pos;
    vec4 light_color;
    vec4 cam_pos;
};

uniform sampler2D duck_map;

void main() {
    vec3 ambient = vec3(1.0, 1.0, 1.0);
    float spec_pow = 0.5f;

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(vec3(light_pos) - frag_pos);  
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * vec3(light_color);

    vec3 view_dir = normalize(vec3(cam_pos) - frag_pos);
    vec3 ref_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, ref_dir), 0.0), 32);
    vec3 specular = spec_pow * spec * vec3(light_color);
    vec4 color = texture(duck_map, ftex);

    frag_color = vec4(((ambient + diffuse + specular) * vec3(color)), 1.0);
}
