#include "Grass.h"

Grass::Grass(int height, int width, glm::vec3 scale, glm::vec3 translate) :
	// stride is 8 + 16 because normals + position + tex = 8 + num floats in model = 16
	Model("objs/grass/grass_bill.obj", scale, translate,  8) 
{
	std::vector<glm::vec3> translates;
	_instances = 0;

	// generate grass model objects 

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, translate);
	for (int y = 0; y < height; y += 3) {
		for (int x = 0; x < width; x += 3) {

			float random_x = -2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2-(-2))));
			float random_y = -2 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2-(-2))));
			glm::vec3 ts = glm::vec3(x + random_x, 0.0f, y + random_y);

			// this is to make sure the grass is on the same height as the terrain.
			ts.y = Terrain::perlin(x, y, model);

			translates.push_back(ts);
			_instances += 1;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * translates.size(), translates.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);

	glBindVertexArray(vao);
	glVertexAttribDivisor(3, 1);
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
