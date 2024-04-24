#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 tex;

uniform mat4 camera_mat;

void main() {
	vec4 pos = camera_mat * vec4(aPos, .0f);
	gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
	tex = vec3(aPos.x, aPos.y, aPos.z);
}