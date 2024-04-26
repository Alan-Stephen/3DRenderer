#include "Water.h"

Water::Water(glm::mat4 model, unsigned int height, unsigned int width, glm::vec3 scale, glm::vec3 translate)
{

    _diffuse = glm::vec4(0.1, 0.5, 1.0, 1.0);
    _specular = glm::vec3(0.1, 0.7, 1.0);
    int shininess = 16;

    _model = glm::scale(model, scale);
    _model = glm::translate(_model, translate);
    // Calculate the number of _verts needed
    int num__verts = height * width * 6; // Each quad has two triangles, each triangle has three _verts

    // Reserve memory for the _verts
    _verts.reserve(num__verts * 3); // Each vertex has three components (x, y, z)

    // Loop to generate _verts for each quad
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
                float x1 = static_cast<float>(x);
                float z1 = static_cast<float>(y);
                float x2 = x1 + 1.0f;
                float z2 = z1 + 1.0f;

                // Add the vertices of the first triangle of the quad
                _verts.push_back(x1); _verts.push_back(0.0f); _verts.push_back(z1); // Vertex 1
                _verts.push_back(x1); _verts.push_back(0.0f); _verts.push_back(z2); // Vertex 3
                _verts.push_back(x2); _verts.push_back(0.0f); _verts.push_back(z1); // Vertex 2

                _verts.push_back(x1); _verts.push_back(0.0f); _verts.push_back(z2); // Vertex 4
                _verts.push_back(x2); _verts.push_back(0.0f); _verts.push_back(z2); // Vertex 6
                _verts.push_back(x2); _verts.push_back(0.0f); _verts.push_back(z1); // Vertex 5
        }
    }

	    glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (_verts.size()), _verts.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
}

void Water::draw(Shader &shader, Camera camera)
{
    shader.bind();
	camera.bind(45, 0.01f, 100000.f, shader, "cameraMat");
	glUniform3fv(shader.get_uniform_location("cam_pos"), 1, glm::value_ptr(camera.get_pos()));
    glUniform4fv(shader.get_uniform_location("diffuse"), 1, glm::value_ptr(_diffuse));
    glUniform3fv(shader.get_uniform_location("specular"), 1, glm::value_ptr(_specular));
    glUniformMatrix4fv(shader.get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(_model));
    glUniform1f(shader.get_uniform_location("time"), static_cast<float>(glfwGetTime()));
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _verts.size());
}

Water::~Water()
{
    std::cout << "DECONSTRUCTING WATER MESH " << _vao << std::endl;
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);

}

glm::mat4 Water::get_model()
{
	return _model;
}
