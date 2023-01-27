#version 440 core
layout(points) in;
layout(triangle_strip, max_vertices=36) out;

const uvec3 verts[] = {
	uvec3(0, 0, 0),
	uvec3(0, 1, 0),
	uvec3(1, 1, 0),
	uvec3(0, 0, 0),
	uvec3(1, 1, 0),
	uvec3(1, 0, 0),

	uvec3(0, 1, 1),
	uvec3(0, 0, 1),
	uvec3(1, 1, 1),
	uvec3(1, 1, 1),
	uvec3(0, 0, 1),
	uvec3(1, 0, 1),

	uvec3(0, 0, 1),
	uvec3(0, 0, 0),
	uvec3(1, 0, 1),
	uvec3(1, 0, 1),
	uvec3(0, 0, 0),
	uvec3(1, 0, 0),

	uvec3(0, 1, 0),
	uvec3(0, 1, 1),
	uvec3(1, 1, 1),
	uvec3(0, 1, 0),
	uvec3(1, 1, 1),
	uvec3(1, 1, 0),

	uvec3(0, 1, 0),
	uvec3(0, 0, 0),
	uvec3(0, 1, 1),
	uvec3(0, 1, 1),
	uvec3(0, 0, 0),
	uvec3(0, 0, 1),

	uvec3(1, 0, 0),
	uvec3(1, 1, 0),
	uvec3(1, 1, 1),
	uvec3(1, 0, 0),
	uvec3(1, 1, 1),
	uvec3(1, 0, 1)
};
 
in VS_OUT {
	vec3 atlas_size;
	vec3 tex;
	vec3 tex_size;
	vec3 nwh;
	vec3 pos;
    vec3 ang;
	vec3 mod_pos;
	mat3 mod_mat;
} gs_in[];

out VS_OUT {
	vec3 atlas_size;
	vec3 tex;
	vec3 tex_size;
	vec3 nwh;
	vec3 pos;
    vec3 ang;
	vec3 mod_pos;
	mat3 mod_mat;
} gs_out;

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

vec3 get_ppos(uint g) {
	return rot_yz(gs_in[0].ang.x) * rot_zx(gs_in[0].ang.y) * rot_xy(gs_in[0].ang.z) * (gs_in[0].mod_pos - gs_in[0].pos + gs_in[0].mod_mat * (verts[g] * gs_in[0].tex_size));
}

in gl_PerVertex {
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
} gl_in[];

out gl_PerVertex {
  vec4 gl_Position;
  float gl_PointSize;
  float gl_ClipDistance[];
};

void main() {
	gs_out.atlas_size = gs_in[0].atlas_size;
	gs_out.tex = gs_in[0].tex;
	gs_out.tex_size = gs_in[0].tex_size;
	gs_out.nwh = gs_in[0].nwh;
	gs_out.pos = gs_in[0].pos;
    gs_out.ang = gs_in[0].ang;
	gs_out.mod_pos = gs_in[0].mod_pos;
	gs_out.mod_mat = gs_in[0].mod_mat;
		
	vec3 k0 = gl_in[0].gl_Position.xyz;
	vec3 m;
	for(uint j = 0; j < 12; j++) {
		for(uint i = 0; i < 3; i++) {
			m = rot_yz(gs_out.ang.x) * rot_zx(gs_out.ang.y) * rot_xy(gs_out.ang.z) * (k0 + gs_out.mod_mat * (verts[3*j + i] * gs_out.tex_size));
			gl_Position = vec4(gs_out.nwh.z*m.x, gs_out.nwh.y*m.y, 0, gs_out.nwh.y*m.z);
			EmitVertex();
		}
		EndPrimitive();
	}
}