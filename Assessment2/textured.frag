#version 330 core

in vec2 tex;

uniform sampler2D diffuseTex;

out vec4 fragColour;

void main()
{
	fragColour = texture(diffuseTex, tex);
}
