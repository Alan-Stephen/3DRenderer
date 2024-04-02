#pragma once
#include "glm/glm.hpp"
#include "Texture.h"

class Material {
private:
	Texture _ambient;
	Texture _specular;
	Texture _diffuse;
	float   _shininess;
public:
	Material(Texture ambient, Texture specular, Texture diffuse, float shininess);
	~Material();
};
