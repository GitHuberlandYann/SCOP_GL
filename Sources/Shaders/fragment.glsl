#version 150 core

in vec3 Color;
in vec2 Texcoord;
flat in int UseTex;
flat in int Tex_index;
flat in int Invert;

out vec4 outColor;
TEX
void main()
{
	OUT
	
	if (Invert == 1) {
		outColor = vec4(1.0, 1.0, 1.0, 2.0) - outColor;
	}
}
