#pragma once
#include "Model.h"
#include "glm/common.hpp"
#include "Mesh.h"
#include "glm/gtc/matrix_transform.hpp"

class Terrain : public Model
{
public:
	Terrain(glm::vec3 scale, glm::vec3 translate, int height, int width);
private:
	int _height;
	int _width;
	float perlin(float x, float y);
	glm::vec3 perlin_norm(int x, int y);
};

