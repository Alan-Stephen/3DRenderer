#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex> &verts, unsigned int material_ref, unsigned int stride) :
	_verts(verts), _material_ref(material_ref)
{
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (verts.size() * 8), verts.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
}

void Mesh::deinit()
{
	std::cout << "DECONSTRUCTING MESH " << _vao << std::endl;
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
}

void Mesh::draw() const
{
	glBindVertexArray(_vao);
	
	glDrawArrays(GL_TRIANGLES, 0, _verts.size());
}

unsigned int Mesh::get_material_ref() const
{
	return _material_ref;
}

unsigned int Mesh::get_vao() const
{
	return _vao;
}

unsigned int Mesh::get_num_verticies() const
{
	return _verts.size();
}
