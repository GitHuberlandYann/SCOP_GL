#version 150 core

in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 scale;

out vec3 Position;

void main()
{
	Position = position;
	gl_Position = proj * view * model * scale * vec4(position, 1.0);
}
