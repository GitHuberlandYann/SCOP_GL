#version 150 core

in vec3 Position;

out vec4 outColor;

void main()
{
	outColor = vec4((Position.x + 0.5) / 2, (Position.y + 0.5) / 2, (Position.z + 0.5) / 2, 1.0);
}
