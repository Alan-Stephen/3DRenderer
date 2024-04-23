#pragma once
#include "Shader.h"
#include <string>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


class Camera
{
public:
	glm::vec3 _position;
	glm::vec3 _orientation = glm::vec3(0.0f,0.0f,1.0f);
	glm::vec3 _up = glm::vec3(0.0f,1.0f,0.0f);

	int _height;
	int _width;

	float _speed = 0.5f;
	float _sensitivity = 100.f;

	bool _firstClick = true;

	Camera(int width, int height, glm::vec3 position);

	void bind(float fov, float nearPlane, float farPlane, Shader& shader, std::string uniform);
	void handleInput(GLFWwindow* window);
	glm::vec3 get_pos() const;
};

