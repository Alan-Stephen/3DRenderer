#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorms;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 iPos;

uniform mat4 model;
uniform mat4 cameraMat;
uniform mat4 light_projection;

out vec2 tex;
out vec3 norm;
out vec3 pos;
out vec4 frag_pos_light;

void main()
{
	tex = aTex;
	norm = aNorms;
	
	pos = vec3(model * vec4(aPos, 1.0f));

	pos +=  iPos;
	frag_pos_light = light_projection * vec4(pos, 1.0f);
	gl_Position =	cameraMat * vec4(pos, 1.0f);

}
