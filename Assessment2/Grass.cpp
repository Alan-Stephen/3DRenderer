#include "Grass.h"

Grass::Grass(int height, int width, glm::vec3 scale, glm::vec3 translate) :
	// stride is 8 + 16 because normals + position + tex = 8 + num floats in model = 16
	Model("objs/grass/grass_bill.obj", scale, translate,  8), _instances(height* width)
{
	std::vector<glm::vec2> translates;

	// generate grass model objects 

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			glm::mat4 model = glm::mat4(1.0f);

			glm::vec2 ts = glm::vec2(x * 5, y*5);
			model = glm::translate(model, translate + glm::vec3(x,0,y));
			model = glm::scale(model, scale);

			// this is to make sure the grass is on the same height as the terrain.
			//ts.y = Terrain::perlin(x, y, model);

			translates.push_back(ts);
		}
	}

	assert(_meshes.size() == 1);
	// we know that grass.obj only has one mesh.

	unsigned int vao = _meshes.at(0).get_vao();
	unsigned int vbo;

	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// bind grass transformation data to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * translates.size(), translates.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

	glBindVertexArray(vao);
	glVertexAttribDivisor(3, 1);

	/*
	std::size_t vec4size = sizeof(glm::vec4);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4size , (void *)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void *)(1 * vec4size));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void *)(2 * vec4size));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void *)(3 * vec4size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	*/
}

void Grass::draw(const Shader& shader) const
{
	shader.bind();
	glUniformMatrix4fv(shader.get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(get_model()));	
	_materials.at(0).bind(shader);
	unsigned int vao = _meshes.at(0).get_vao();
	glBindVertexArray(vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 12, _instances);
	return;

}

glm::mat4 Grass::get_model() const
{
	return _model;
}
