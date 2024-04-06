#include "Material.h"
#include <string>

Material::Material(const Texture &ambient, const Texture &specular, const Texture &diffuse, float shininess, const std::string &name) :
	_ambient(ambient), _specular(specular), _diffuse(diffuse), _shininess(shininess), _name(name)
{}

void Material::deinit()
{
	_diffuse.deinit();
	_specular.deinit();
	_ambient.deinit();

}

void Material::bind(const Shader &shader) const
{
	_diffuse.bind(0);
	_specular.bind(1);
	glUniform1f(shader.get_uniform_location("shininess"), _shininess);
}

std::string Material::get_name() const
{
	return _name;
}

