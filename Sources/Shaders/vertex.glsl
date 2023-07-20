#version 150 core

in vec3 position;
in vec3 color;
in vec2 texcoord;

uniform int color_mode;
uniform int tex_index;
uniform int last_tex;
uniform int invert_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 scale;

out vec3 Color;
out vec2 Texcoord;
flat out int UseTex;
flat out int Tex_index;
flat out int Invert;

void main()
{
	Texcoord = texcoord;
	UseTex = 0;
	Tex_index = tex_index;
	Invert = invert_color;
	if (color_mode == 0) {
		Color = vec3(1.0, 1.0, 1.0);
	} else if (color_mode == 1) {
		Color = color;
	} else if (color_mode == 2) {
		UseTex = 1;
	} else if (color_mode == 3) {
		Texcoord = vec2(position.x + 0.5 + position.z / 10.0, position.y + 0.5 + position.z / 10.0);
		UseTex = 1;
		Tex_index = last_tex;
	} else if (color_mode == 4) {
		Color = vec3((position.x + 0.5) / 2, (position.y + 0.5) / 2, (position.z + 0.5) / 2);
	} else { // not supposed to happen
		Color = vec3(0.0, 0.0, 1.0);
	}

	gl_Position = proj * view * model * scale * vec4(position, 1.0);
}
