#include "MaterialContainer.h"
#include "Shader.h"

MaterialContainer::MaterialContainer(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, Texture diffuseTex, Texture  specularTex) :
	_ambient(ambient), _diffuse(diffuse), _specular(specular), _diffuseTex(diffuseTex), _specularTex(specularTex)
{
}

MaterialContainer::~MaterialContainer() {}

void MaterialContainer::bind(Shader shader)
{
	if (_diffuseTex.isValid() && _specularTex.isValid()) {
		_diffuseTex.bind(0);
		glUniform1i(shader.getUniformLocation("diffuseTex"), 0);

		_diffuseTex.bind(1);
		glUniform1i(shader.getUniformLocation("specularTex"), 1);
		glUniform1i(shader.getUniformLocation("hasTexture"), true);
	}
	else {

		glUniform3f(shader.getUniformLocation("diffuseColour"), _diffuse.x,_diffuse.y,_diffuse.z);
		glUniform3f(shader.getUniformLocation("specularColour"), _specular.x,_specular.y,_specular.z);
		glUniform1i(shader.getUniformLocation("hasTexture"), false);
	}
}
