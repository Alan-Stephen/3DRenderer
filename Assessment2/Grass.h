#pragma once
#include "glm/glm.hpp"
#include "Material.h"

class Grass
{
public:
	Grass(int height, int width, glm::vec3 scale, glm::vec3 translate);
private:
	unsigned int _vao;
	Material _mat;
	unsigned int _vbo;
};

