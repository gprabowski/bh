#version 460

in vec2 frag_pos;

out vec4 frag_color;

layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 bh_data;
    vec4 exec_data;
};

uniform samplerCube skybox;

float ptpvd(in vec3 a, in vec3 v, in vec3 p) {
    return length((p - a) - (dot((p - a),v) * v));
}

float inner_formula(float w, float m, float b) {
    return 1.0 - w * w * (1.0 - 2.0 * (m / b) * w);
}

float derivative(float w, float m, float b) {
    return -2.0 * w + 6.0 * w * w * (m / b);
}

vec3 rotate_vector(vec3 v, vec3 a, float angle) {
    const float s = sin(0.5 * angle);
    vec4 q = vec4(a.x * s, a.y * s, a.z * s, cos(angle * 0.5));
    return v + 2.0*cross(cross(v, q.xyz ) + q.w*v, q.xyz);
}

void main() {
    mat4 inv_proj = inverse(proj);
    mat4 inv_view = inverse(view);

    float projXInv = 1 / proj[0][0];
    float projYInv = 1 / proj[1][1];

    vec4 viewVec = vec4(projXInv * frag_pos.x, projYInv * frag_pos.y, 1, 0);

    vec3 bh_pos = bh_data.xyz;
    float bh_mass = bh_data.w;

    float b = ptpvd(vec3(0, 0, 0), normalize(viewVec.xyz), bh_pos);
    float ratio = bh_mass / b;

    if(ratio > 0.17) {
        frag_color = vec4(1, 0.0, 0.0, 1.0);
        return;
    } else if(ratio < 1e-6) {
        viewVec = vec4(normalize(vec3(inv_view * viewVec)), 0);
        frag_color = texture(skybox, viewVec.xyz);
        return;
    } else {
        // newton
        float w1 = 1.0;
        for(uint i = 0; i < 50; ++i) {
           w1 -= inner_formula(w1, bh_mass, b) / derivative(w1, bh_mass, b);
        }

        float phi = 0.0;
        float w_step = (w1) / 1000.f;

        for (uint i = 0u; i < 1000; i++) {
            float w = w_step * float(i);
            float f_a = 2.0f / sqrt(inner_formula(w, bh_mass, b));
            phi += w_step * f_a;
        }

        phi = phi - 3.1415926535897932384626433832795;

        vec3 final_vec = rotate_vector(normalize(vec3(viewVec)), normalize(cross(bh_pos, viewVec.xyz)), phi);
        
        final_vec = normalize(vec3(inv_view * vec4(final_vec, 0)));
        if(any(isnan(phi))) {
            frag_color = vec4(0, 0, 1, 1);
        } else {
            frag_color = texture(skybox, normalize(final_vec));
        }
    }
}
