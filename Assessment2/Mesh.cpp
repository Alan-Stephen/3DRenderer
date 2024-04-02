#include "Mesh.h"
#include "Model.h"
#include "Shader.h"

Mesh::Mesh(const std::vector<Vertex> &verts, unsigned int material_ref) :
	_verts(verts), _material_ref(material_ref)
{
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (verts.size() * 8), verts.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
}

void Mesh::draw(const Shader& shader)
{
	glBindVertexArray(_vao);
	
	glDrawArrays(GL_TRIANGLES, 0, _verts.size());
}
