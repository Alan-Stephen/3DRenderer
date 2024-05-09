#include "LightCube.h"
#include <iostream>

LightCube::LightCube(glm::vec3 translate, glm::vec3 scale, glm::vec3 light_col) 
    : _light_col(light_col)
{
    _model = glm::mat4(1.0f);
    _model = glm::translate(_model, translate);
    _model = glm::scale(_model, scale);

     _verts = {
    -1.0f,-1.0f,-1.0f, 
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f,-1.0f, 
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, 
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
     };

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);

	glBindVertexArray(_vao);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (_verts.size()), _verts.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void LightCube::draw(Shader& shader, const Camera *camera)
{

    shader.bind();
	camera->bind(shader, "camera_mat");
    glUniformMatrix4fv(shader.get_uniform_location("model"), 1, GL_FALSE, glm::value_ptr(_model));
    glUniform3f(shader.get_uniform_location("light_col"), _light_col.r, _light_col.g, _light_col.b);
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, _verts.size());
}

LightCube::~LightCube()
{
    std::cout<< "[DEINIT] LIGHT CUBE WITH VAO: " << _vao << ", VBO: " << _vbo;
    glDeleteBuffers(1,&_vbo);
    glDeleteVertexArrays(1, &_vao);
}
