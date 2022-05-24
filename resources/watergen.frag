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

uniform sampler2D height_texture_last;
uniform sampler2D height_texture_before_last;

uniform float delta_time;
uniform vec2 discordance;
uniform vec2 duck;

const float N = 256;
const float h = 2.0 / (N - 1);
const float c = 1.0;
const float dt = 1.0 / N;
const float dt_2 = 2.0 / N;

void main() {
    float l = min(1.0 - abs(frag_pos.x), 1.0 - abs(frag_pos.y));
    float d = 0.95 * min(1.0, l / 0.2);
    d = 0.99;
    float A = ((dt/h) * (dt/h));
    float B = 2 - 4 * A;
    vec2 texpos = (frag_pos.xy + vec2(1.0, 1.0)) * 0.5;

    float zright = texture(height_texture_last, texpos + vec2(dt, 0.0)).x;
    float zleft = texture(height_texture_last, texpos + vec2(-dt, 0.0)).x;
    float zdown = texture(height_texture_last, texpos+ vec2(0.0, -dt)).x;
    float zup = texture(height_texture_last, texpos + vec2(0.0, dt)).x;

    vec4 _height = vec4(d * ( A * (
                    zright + zleft + zdown + zup
                ) + B * texture(height_texture_last, texpos).x 
                - texture(height_texture_before_last, texpos).x), 

            0.0, 0.0, 1.0);

    if(length(frag_pos.xy - duck) < (2 * dt) || length(frag_pos.xy - discordance) < (2 * dt)) _height.x += 0.5;
    height = _height;

    // normal
    float nx = (zleft - zright);
    float ny = (zdown - zup);
    float nz = 2 * dt;
    normal = vec4(normalize(vec3(nx, ny, nz)), 1.0);
}
