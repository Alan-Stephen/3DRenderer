#pragma once

#include <vector>
#include "Shader.h"
#include "glm/glm.hpp"

struct Vertex {
	Vertex(glm::vec3 position, glm::vec2 tex, glm::vec3 normal) : 
		position(position), normal(normal), tex(tex)
	{}
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
	Mesh(const std::vector<Vertex> &verts, unsigned int material_ref, unsigned int stride);
	void deinit();
	void draw() const;
	unsigned int get_material_ref() const;
	unsigned int get_vao() const;
	unsigned int get_num_verticies() const;
};

