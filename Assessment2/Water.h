#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"
#include <vector>
#include "GL/gl3w.h"
#include <iostream>
#include "glfw/glfw3.h"
#include "Material.h"

class Water
{
public:
	// width and height are for vertiices, scale is for size.
	Water(glm::mat4 model, unsigned int height, unsigned int width, glm::vec3 scale, glm::vec3 translate);
	void draw(Shader &shader);
	~Water();
	glm::mat4 get_model();
private:
	unsigned int _vbo;
	unsigned int _vao;
	Material _material;
	std::vector<float> _verts;
	glm::mat4 _model = glm::mat4(1.f);
};
