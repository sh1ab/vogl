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

in VS_OUT {
	vec3 tex;
	vec3 tex_size;
	vec3 mod_pos;
	vec4 mod_pol;
} gs_in[];

out VS_OUT {
	vec3 tex;
	vec3 tex_size;
	vec3 mod_pos;
	vec4 mod_pol;
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
	gs_out.tex = gs_in[0].tex;
	gs_out.tex_size = gs_in[0].tex_size;
	gs_out.mod_pos = gs_in[0].mod_pos;
	gs_out.mod_pol = gs_in[0].mod_pol;

	mat3 mm_rot = rot_yz(ang_x) * rot_zx(ang_y) * rot_xy(ang_z);
	mat3 mm_mod_rot = rot_yz(gs_out.mod_pol.x) * rot_zx(gs_out.mod_pol.y) * rot_xy(gs_out.mod_pol.z);

	for(uint hh = 0; hh < 12; hh++) {
		for(uint jj = 0; jj < 3; jj++) {
			gl_Position = vec4(mm_rot*(gl_in[0].gl_Position.xyz + mm_mod_rot * (gs_out.mod_pol.w*gs_out.tex_size*verts[3*hh+jj])), 0).xywz*vec4(h, w, 0, w);
			EmitVertex();
		}
		EndPrimitive();
	}
}