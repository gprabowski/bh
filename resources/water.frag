#version 460

in vec4 color;
in vec3 fragtex;
in vec3 world_pos;

out vec4 frag_color;

uniform sampler2D normal_map;
uniform sampler2D height_map;
uniform samplerCube skybox;

vec3 intersect_ray(vec3 p, vec3 dir) {
	float tx = max((-p.x - 1) / dir.x, (-p.x + 1) / dir.x);
	float ty = max((-p.y - 1) / dir.y, (-p.y + 1) / dir.y);
	float tz = max((-p.z - 1) / dir.z, (-p.z + 1) / dir.z);
	
	return p + dir * min(tx, min(ty, tz));
}

float fresnel(vec3 n, vec3 v) {
	float n1 = 1.f, n2 = 4.f / 3.f;
	float costheta = max(dot(normalize(n), normalize(v)), 0.0);
	float F0 = pow((n2 - n1) / (n2 + n1), 2);
	return F0 + (1 - F0) * pow((1 - costheta), 5);
}


layout (std140) uniform common_block {
    mat4 proj;
    mat4 view;
    vec4 light_pos;
    vec4 light_color;
    vec4 cam_pos;
};

void main() {
    float height = texture(height_map, fragtex.xy).x;
    vec3 norm = texture(normal_map, fragtex.xy).xzy;

    float n1 = 1.f, n2 = 4.f / 3.f;
	float ratio = n1 / n2;
	vec3 view_vec = normalize(cam_pos.xyz - world_pos);

	if (dot(norm, view_vec) < 0) {
		ratio = n2 / n1;
		norm = -1.f * norm;
	}

	vec3 reflected = (reflect(-view_vec, norm));
	vec3 tex_refl = intersect_ray(world_pos, reflected);

	vec3 refracted = (refract(-view_vec, norm, ratio));
	vec3 tex_refr = intersect_ray(world_pos, refracted);


	vec3 crefl = vec3(texture(skybox, normalize(tex_refl)));
	vec3 crefr = vec3(texture(skybox, normalize(tex_refr)));

	float fr_refl = fresnel(norm, view_vec);
	float fr_refr = 1.0f - fr_refl;
    if(length(refracted) < 1e-3) {
		fr_refr = 0.0f;
		fr_refl = 1.0f;
    }
    vec3 res = fr_refl * crefl + fr_refr * crefr;
	frag_color = vec4(pow(res.x, 0.4545), pow(res.y, 0.4545), pow(res.z, 0.4545), 1.0f);

}
