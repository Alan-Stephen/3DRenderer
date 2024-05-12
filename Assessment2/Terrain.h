#pragma once
#include "Model.h"
#include "glm/common.hpp"
#include "Mesh.h"
#include "glm/gtc/matrix_transform.hpp"

class Terrain : public Model
{
public:
	Terrain(glm::vec3 scale, glm::vec3 translate, int height, int width);
	static float perlin(float a, float b, glm::mat4 model);
private:
	int _height;
	int _width;
	static glm::vec2 rand_grad(int ix, int iy);
	glm::vec3 perlin_norm(int x, int y);
	static float grid_grad(int ix, int iy, float x, float y);
	static float lerp(float start, float end, float t);
};

