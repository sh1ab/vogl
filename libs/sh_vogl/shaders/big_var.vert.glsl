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

const uvec3 vertices[] = {
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

#define _W tex_size_x
#define _H tex_size_y
#define _D tex_size_z

vec2 rot(in vec2 inp, in float alpha) {
    return vec2(
		inp.x*cos(alpha) - inp.y*sin(alpha), 
		inp.x*sin(alpha) + inp.y*cos(alpha)
	);
}

out gl_PerVertex{
	vec4 gl_Position;
};

void main() {
    vec3 mod_pos = vec3(mod_pos_x, mod_pos_y, mod_pos_z);
	vec3 pos = vec3(pos_x, pos_y, pos_z);
	mat3 mod_mat = mat3(
		mod_mat_x_x, mod_mat_x_y, mod_mat_x_z,
		mod_mat_y_x, mod_mat_y_y, mod_mat_y_z,
		mod_mat_z_x, mod_mat_z_y, mod_mat_z_z);

	vec3 model;
	model = vertices[gl_VertexID]*uvec3(_W, _H, _D);

	model = mod_mat * model + mod_pos;

	model -= pos;

	model.xy = rot(model.xy, ang_z);
	model.zx = rot(model.zx, ang_y);
	model.yz = rot(model.yz, ang_x);

	gl_Position = vec4(h*model.x, w*model.y, 0, w*model.z);
}