#version 440 core

layout(location = 0) in vec3 atex;
layout(location = 1) in vec3 atexsize;
layout(location = 2) in vec3 amodpos;
layout(location = 3) in vec4 amodpol;

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

out VS_OUT {
	vec3 tex;
	vec3 tex_size;
	vec3 mod_pos;
	vec4 mod_pol;
} vs_out;

out gl_PerVertex{
	vec4 gl_Position;
};

void main() {
	vs_out.tex = atex;
	vs_out.tex_size = atexsize;
    vs_out.mod_pos = amodpos;
	vs_out.mod_pol = amodpol;

	vec3 hh = amodpos - vec3(pos_x, pos_y, pos_z);
	gl_Position = vec4(hh, 0);
}