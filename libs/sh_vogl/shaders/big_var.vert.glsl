#version 440 core

//layout(location = 0) in vec3 atex;
//layout(location = 1) in vec3 atexsize;
//layout(location = 2) in vec3 amodpos;
//layout(location = 3) in mat3 amodmat;

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

out VS_OUT {
	vec3 atlas_size;
	vec3 tex;
	vec3 tex_size;
	vec3 nwh;
	vec3 pos;
    vec3 ang;
	vec3 mod_pos;
	mat3 mod_mat;
} vs_out;

out gl_PerVertex{
	vec4 gl_Position;
};

void main() {
	vs_out.atlas_size = vec3(atlas_size_x, atlas_size_y, atlas_size_z);
	//vs_out.tex = atex;
	vs_out.tex = vec3(tex_x, tex_y, tex_z);
	//vs_out.tex_size = atexsize;
	vs_out.tex_size = vec3(tex_size_x, tex_size_y, tex_size_z);
	vs_out.nwh = vec3(n, w, h);
	vs_out.pos = vec3(pos_x, pos_y, pos_z);
	vs_out.ang = vec3(ang_x, ang_y, ang_z);
    //vs_out.mod_pos = amodpos;
    vs_out.mod_pos = vec3(mod_pos_x, mod_pos_y, mod_pos_z);
	//vs_out.mod_mat = amodmat;
	vs_out.mod_mat = mat3(mod_mat_x_x, mod_mat_x_y, mod_mat_x_z, mod_mat_y_x, mod_mat_y_y, mod_mat_y_z, mod_mat_z_x, mod_mat_z_y, mod_mat_z_z);

	gl_Position = vec4(vs_out.mod_pos - vs_out.pos, 1);
}