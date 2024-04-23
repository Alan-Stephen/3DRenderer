#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include <vector>
#include "GL/gl3w.h"
#include <iostream>
#include "glfw/glfw3.h"

class Water
{
public:
	Water(glm::mat4 model, unsigned int height, unsigned int width);
	void draw(Shader &shader);
	~Water();
	glm::mat4 get_model();
private:
	unsigned int _vbo;
	unsigned int _vao;
	std::vector<float> _verts;
	glm::mat4 _model = glm::mat4(1.f);
};

