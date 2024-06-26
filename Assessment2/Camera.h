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

	float _speed = 0.5f;
	float _sensitivity = 100.f;
	glm::vec3 _up = glm::vec3(0.0f,1.0f,0.0f);

	bool _firstClick = true;

	Camera(int width, int height, glm::vec3 position, float fov, float near_plane, float farPlane);

	virtual void bind(Shader& shader, std::string uniform) const;
	virtual glm::mat4 get_camera_mat();
	virtual void handleInput(GLFWwindow* window);
	virtual glm::vec3 get_pos() const;
private:
	glm::vec3 _position;
	glm::vec3 _orientation = glm::vec3(0.0f,0.0f,1.0f);
protected:
	float _fov;
	float _near_plane;
	float _far_plane;
	int _height;
	int _width;
};

