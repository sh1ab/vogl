#version 440 core

layout(std140, binding = 3) uniform inputs{
	uint atlas_size_x;
	uint atlas_size_y;
	uint atlas_size_z;

	float n;
	float w;
	float h;

	float pos_x;
	float pos_y;
	float pos_z;

	float ang_x;
	float ang_y;
	float ang_z;
};

out vec4 frag_color;
uniform sampler3D vox_tex;

in VS_OUT {
	vec3 tex;
	vec3 tex_size;
	vec3 mod_pos;
	vec4 mod_pol;
} fs_in;

float par_intersect(in vec3 neg_ro, in vec3 inv_rd, in vec3 v) {
    neg_ro = inv_rd * (neg_ro + vec3(inv_rd.x < 0, inv_rd.y < 0, inv_rd.z < 0) * v);
    return max(max(neg_ro.x, neg_ro.y), max(neg_ro.z, 0));
}
float cube_intersect(in vec3 neg_ro, in vec3 inv_rd) {
    neg_ro = inv_rd * (neg_ro + vec3(inv_rd.x < 0, inv_rd.y < 0, inv_rd.z < 0));
    return max(max(neg_ro.x, neg_ro.y), max(neg_ro.z, 0));
}

mat3 rot_xy(in float alpha) {
    return mat3(
		 cos(alpha), sin(alpha), 0,
		-sin(alpha), cos(alpha), 0,
		 0         , 0         , 1   
	);
}

mat3 rot_yz(in float alpha) {
    return mat3(
        1,           0,          0,
		0,  cos(alpha), sin(alpha), 
		0, -sin(alpha), cos(alpha)
	);
}

mat3 rot_zx(in float alpha) {
    return mat3(
		cos(alpha), 0, -sin(alpha), 
                 0, 1,           0,
	    sin(alpha), 0,  cos(alpha)
	);
}

struct ray_hit {
    ivec3 norm;
    vec4 col;
    float l;
};

ivec3 ppp(vec3 v, vec3 w) { return ivec3(v.x <= 0 && w.x <= 0, v.y <= 0 && w.y <= 0, v.z <= 0 && w.z <= 0); }

#define voxel(__x, __y, __z) texture(vox_tex, vec3((fs_in.tex.x + (__x))/float(atlas_size_x), (fs_in.tex.y + (__y))/float(atlas_size_y), (fs_in.tex.z + (__z))/float(atlas_size_z)))
bool cast_ray(in vec3 ro, in vec3 rd, out ray_hit hit) {
    vec3 ro_0 = ro;

    vec3 inv_rd = 1.0 / rd;

    ro += rd * (par_intersect(-ro, inv_rd, fs_in.tex_size) + 0.001);

    if (ro.x < 0 || ro.x >= fs_in.tex_size.x || ro.y < 0 || ro.y >= fs_in.tex_size.y || ro.z < 0 || ro.z >= fs_in.tex_size.z)
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
            } else if (vox_pos.x < 0 || vox_pos.x >= fs_in.tex_size.x || vox_pos.y < 0 || vox_pos.y >= fs_in.tex_size.y || vox_pos.z < 0 || vox_pos.z >= fs_in.tex_size.z) {
                hit.l = cube_intersect(vox_pos - ro_0, inv_rd);
                return true;
            }
        }

        hit.norm = step * ppp(tmax - tmax.yzx, tmax - tmax.zxy);
        vox_pos -= hit.norm;

        if (vox_pos.x < 0 || vox_pos.x >= fs_in.tex_size.x || vox_pos.y < 0 || vox_pos.y >= fs_in.tex_size.y || vox_pos.z < 0 || vox_pos.z >= fs_in.tex_size.z)
            return false;

        tmax -= inv_rd * hit.norm;
    }

    return false;
}
#undef voxel

void main() {
    mat3 inv_mod_mat = rot_xy(-fs_in.mod_pol.z) * rot_zx(-fs_in.mod_pol.y) * rot_yz(-fs_in.mod_pol.x);

	vec3 rd = normalize(vec3(2*gl_FragCoord.xy-vec2(w, h), h));

    rd = rot_xy(-ang_z)*rot_zx(-ang_y)*rot_yz(-ang_x)*rd;

    rd = inv_mod_mat * rd;

    vec3 ro = vec3(pos_x, pos_y, pos_z) - fs_in.mod_pos;
    ro = inv_mod_mat * ro;
    ro *= 1.0 / fs_in.mod_pol.w;

    ray_hit hit;

    bool intersect = cast_ray(ro, rd, hit);

    if (!intersect || hit.col.a < 0.0001)
    	discard;
        
    gl_FragDepth = (hit.l - n) / hit.l;
    frag_color = hit.col;
}