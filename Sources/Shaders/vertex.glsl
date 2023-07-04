#version 150 core

in vec3 position;
in vec3 color;
in vec2 texcoord;

uniform int color_mode;
uniform int tex_index;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 scale;

out vec3 Color;
out vec2 Texcoord;
out vec2 Tex_index;

void main()
{
	Tex_index = vec2(0.0, 0.0);
	if (color_mode == 0) {
		Color = vec3(1.0, 1.0, 1.0);
	} else if (color_mode == 1) {
		Color = color;
	} else if (color_mode == 2) {
		Color = vec3(1.0, 0.0, 0.0);
		Tex_index = vec2(1.0, tex_index);
	} else if (color_mode == 3) {
		Color = vec3((position.x + 0.5) / 2, (position.y + 0.5) / 2, (position.z + 0.5) / 2);
	}
	Texcoord = texcoord;
	gl_Position = proj * view * model * scale * vec4(position, 1.0);
}
