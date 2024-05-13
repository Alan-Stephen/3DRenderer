#include "Grass.h"

Grass::Grass(int height, int width, glm::vec3 scale, glm::vec3 translate) :
	// stride is 8 + 16 because normals + position + tex = 8 + num floats in model = 16
	Model("objs/tree/tree.obj", scale, translate,  8) 
{
	std::vector<glm::vec3> translates;
	_instances = 0;

	_model = glm::rotate(_model, glm::radians(270.f), glm::vec3(1.0, 0, 0));

	// generate grass model objects 

	glm::mat4 model = glm::mat4(1.0f);

	model = glm::translate(model, translate);
	
	for (int y = 0; y < height; y += 20) {
		for (int x = 0; x < width; x += 20) {

			float random_x = -10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-2))));
			float random_y = -10 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (10 - (-2))));
			glm::vec3 ts = glm::vec3(x + random_x, 0.0f, y + random_y);

			// this is to make sure the grass is on the same height as the terrain.
			ts.y = Terrain::perlin(x + random_x, y + random_y, model);

			translates.push_back(ts);
			_instances += 1;
		}
	}

	//assert(_meshes.size() == 1);
	// we know that grass.obj only has one mesh.

	for (int i = 0; i < _meshes.size(); i++) {
		unsigned int vao = _meshes.at(i).get_vao();
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
}

void Grass::draw(const Shader& shader) const
{
	shader.bind();
	glUniformMatrix4fv(shader.get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(get_model()));	

	for (int i = 0; i < _meshes.size(); i++) {
		_materials.at(_meshes.at(i).get_material_ref()).bind(shader);
		unsigned int vao = _meshes.at(i).get_vao();
		glBindVertexArray(vao);
		glDrawArraysInstanced(GL_TRIANGLES, 0, _meshes.at(i).get_num_verticies(), _instances);
	}
}

glm::mat4 Grass::get_model() const
{
	return _model;
}
