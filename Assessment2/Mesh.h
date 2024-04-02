#pragma once

#include <vector>
#include "Shader.h"
#include "glm/glm.hpp"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex;
};

class Mesh
{
private:
	std::vector<Vertex> _verts;
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _material_ref;
public:
	Mesh(const std::vector<Vertex> &verts, unsigned int material_ref);
	~Mesh();
	void draw(const Shader& shader);
};

