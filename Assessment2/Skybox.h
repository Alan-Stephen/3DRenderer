#pragma once
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GL/gl3w.h"
#include <iostream>
#include "Texture.h"

class Skybox {
public:
	Skybox(const Shader &shader);
	void draw(const Shader &shader, const glm::mat4 &camera_mat);
	~Skybox();
private:
	unsigned int _VAO;
	unsigned int _texture;
};
