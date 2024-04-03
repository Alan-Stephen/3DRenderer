#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorms;
layout(location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 cameraMat;

out vec2 tex;
out vec3 norm;
out vec3 pos;

void main()
{
	gl_Position = cameraMat * model * vec4(aPos, 1.f);
	tex = aTex;
	norm = aNorms;
	pos = aPos;
}
