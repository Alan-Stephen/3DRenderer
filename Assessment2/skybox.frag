#version 330 core

out vec4 frag_colour;

in vec3 tex;

uniform samplerCube skybox;

void main() {
	frag_colour = texture(skybox,tex);
}