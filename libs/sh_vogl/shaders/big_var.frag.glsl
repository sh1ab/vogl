#version 440 core

layout(std140, binding = 3) uniform inputs{
	uint atlas_size_x;
	uint atlas_size_y;
	uint atlas_size_z;

    uint tex_x;
    uint tex_y;
    uint tex_z;
    
    uint tex_size_x;
    uint tex_size_y;
    uint tex_size_z;

	float n;
	float w;
	float h;

	float pos_x;
	float pos_y;
	float pos_z;

	float ang_x;
	float ang_y;
	float ang_z;

	float mod_pos_x;
	float mod_pos_y;
	float mod_pos_z;

	float mod_mat_x_x;
	float mod_mat_x_y;
	float mod_mat_x_z;

	float mod_mat_y_x;
	float mod_mat_y_y;
	float mod_mat_y_z;

	float mod_mat_z_x;
	float mod_mat_z_y;
	float mod_mat_z_z;
};

out vec4 frag_color;
uniform sampler3D tex;


float par_intersect(in vec3 neg_ro, in vec3 inv_rd, in vec3 v) {
    neg_ro = inv_rd * (neg_ro + vec3(inv_rd.x < 0, inv_rd.y < 0, inv_rd.z < 0) * v);
    return max(max(neg_ro.x, neg_ro.y), max(neg_ro.z, 0));
}
float cube_intersect(in vec3 neg_ro, in vec3 inv_rd) {
    neg_ro = inv_rd * (neg_ro + vec3(inv_rd.x < 0, inv_rd.y < 0, inv_rd.z < 0));
    return max(max(neg_ro.x, neg_ro.y), max(neg_ro.z, 0));
}

vec2 rot(in vec2 inp, in float alpha) {
    return vec2(
		inp.x*cos(alpha) - inp.y*sin(alpha), 
		inp.x*sin(alpha) + inp.y*cos(alpha)
	);
}

struct ray_hit {
    ivec3 norm;
    vec4 col;
    float l;
};

ivec3 ppp(vec3 v, vec3 w) { return ivec3(v.x <= 0 && w.x <= 0, v.y <= 0 && w.y <= 0, v.z <= 0 && w.z <= 0); }

#define W tex_size_x
#define H tex_size_y
#define D tex_size_z
#define voxel(__x, __y, __z) texture(tex, vec3((tex_x + (__x))/float(atlas_size_x), (tex_y + (__y))/float(atlas_size_y), (tex_z + (__z))/float(atlas_size_z)))
bool cast_ray(in vec3 ro, in vec3 rd, out ray_hit hit) {
    vec3 ro_0 = ro;

    vec3 inv_rd = 1.0 / rd;

    ro += rd * (par_intersect(-ro, inv_rd, vec3(W, H, D)) + 0.001);

    if (ro.x < 0 || ro.x >= W || ro.y < 0 || ro.y >= H || ro.z < 0 || ro.z >= D)
        return false;

    ivec3 vox_pos = ivec3(ro);
    ivec3 step = -ivec3(sign(rd));
    vec3 tmax = inv_rd*((1 - step) * 0.5 + vox_pos - ro);

    while (true) {
        vec4 color = voxel(vox_pos.x, vox_pos.y, vox_pos.z);

        if (color.a > 0.0001) {
            hit.col = (color + hit.col) / (1 + color * hit.col);

            if (hit.col.a > 0.99) {
                hit.l = cube_intersect(vox_pos - ro_0, inv_rd);
                hit.col.a = 1;
                return true;
            }
        }

        hit.norm = step * ppp(tmax - tmax.yzx, tmax - tmax.zxy);
        vox_pos -= hit.norm;

        if (vox_pos.x < 0 || vox_pos.x >= W || vox_pos.y < 0 || vox_pos.y >= H || vox_pos.z < 0 || vox_pos.z >= D)
            return false;

        tmax -= inv_rd * hit.norm;
    }

    return false;
}
#undef voxel
#undef D
#undef H
#undef W

void main() {
    mat3 inv_mod_mat = inverse(mat3(
        mod_mat_x_x, mod_mat_x_y, mod_mat_x_z,
        mod_mat_y_x, mod_mat_y_y, mod_mat_y_z,
        mod_mat_z_x, mod_mat_z_y, mod_mat_z_z));

	vec3 rd = normalize(vec3(2*gl_FragCoord.xy-vec2(w, h), h));

	rd.yz = rot(rd.yz, -ang_x);
	rd.zx = rot(rd.zx, -ang_y);
	rd.xy = rot(rd.xy, -ang_z);

    rd = inv_mod_mat * rd;

    float inv_l = length(rd);
    if (inv_l < 0.0001) {
        gl_FragDepth = 0.1f; 
        frag_color = vec4(1, 1, 1, 1);
        return;
		discard;
    }
    inv_l = 1.0 / inv_l;
    rd = rd*inv_l;

    vec3 ro = vec3(pos_x - mod_pos_x, pos_y - mod_pos_y, pos_z - mod_pos_z);
    ro = inv_mod_mat * ro;

    ray_hit hit;

    bool intersect = cast_ray(ro, rd, hit);

    if (!intersect || hit.col.a < 0.0001)
		discard;

    hit.l *= inv_l;
    gl_FragDepth = (hit.l - n) / hit.l;
    frag_color = hit.col;
}