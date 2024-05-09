#version 330 core

out vec4 frag_col;

uniform vec3 light_col;

void main()
{
	frag_col = vec4(light_col, 1.0f);
}