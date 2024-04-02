#pragma once
#include "glm/glm.hpp"
#include "GL/gl3w.h"
#include "Texture.h"
#include "Shader.h"
class MaterialContainer
{
public:
	MaterialContainer(
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		Texture diffuseTex,
		Texture specularTex
	);

	~MaterialContainer();

	void bind(Shader shaderId);
private:
	 glm::vec3 _ambient;
	 glm::vec3 _diffuse;
	 glm::vec3 _specular;
	 Texture   _diffuseTex;
	 Texture   _specularTex;
};

