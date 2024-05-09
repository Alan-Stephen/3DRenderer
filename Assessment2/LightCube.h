#pragma once
#include "glm/common.hpp"
#include "Shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GL/gl3w.h"
#include <vector>
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

class LightCube
{
public:
	LightCube() {};
	LightCube(glm::vec3 translate, glm::vec3 scale, glm::vec3 light_col);
	void draw(Shader& shader, const Camera *camera);
	~LightCube();
private:
	std::vector<float> _verts;
	glm::vec3 _light_col;
	glm::mat4 _model;
	unsigned int _vao;
	unsigned int _vbo;
};

