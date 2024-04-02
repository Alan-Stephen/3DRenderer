#include "Material.h"

Material::Material(Texture ambient, Texture specular, Texture diffuse, float shininess) :
	_ambient(ambient), _specular(specular), _diffuse(diffuse), _shininess(shininess)
{}

Material::~Material()
{
	_diffuse.deinit();
	_specular.deinit();
	_ambient.deinit();
}

