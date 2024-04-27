#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position, float fov, float near_plane, float far_plane) : 
	_width(width), _height(height), _position(position), _fov(fov), _near_plane(near_plane), _far_plane(far_plane)
{}

void Camera::bind(Shader &shader,std::string uniform) const
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(_position, _position + _orientation, _up);
	projection = glm::perspective(glm::radians(_fov), (float)(_width / _height), _near_plane, _far_plane);

	glUniformMatrix4fv(shader.get_uniform_location(uniform),1, GL_FALSE, glm::value_ptr(projection * view));
}

glm::mat4 Camera::get_camera_mat()
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(_position, _position + _orientation, _up);
	projection = glm::perspective(glm::radians(_fov), (float)(_width / _height), _near_plane, _far_plane);

	return projection * view;
}

void Camera::handleInput(GLFWwindow* window)
{
	// y movement shouldn't be affected by orientation only using L-SHIFT and SPACE
	// like minecraft creative flight.

	glm::vec3 orient_without_y = glm::vec3(_orientation.x, 0.0f, _orientation.z);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		_position += _speed * orient_without_y;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		_position += _speed * -glm::normalize(glm::cross(_orientation, _up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		_position += _speed * -orient_without_y;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		_position += _speed * glm::normalize(glm::cross(_orientation, _up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		_position += _speed * _up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		_position += _speed * -_up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		_speed = 2.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		_speed = 1.1f;
	}


	// handle mouse inputs
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	double mouse_x;
	double mouse_y;

	glfwGetCursorPos(window, &mouse_x, &mouse_y);

	// normalize mouse coords  to center of screen
	float rot_x = _sensitivity * (float)(mouse_y - (_height / 2)) / _height;
	float rot_y = _sensitivity * (float)(mouse_x - (_width / 2)) / _width;

	glm::vec3 new_orientation = glm::rotate(_orientation, glm::radians(-rot_x), glm::normalize(glm::cross(_orientation, _up)));

	// prevents camera from flipping in on itself. 
	if (abs(glm::angle(new_orientation, _up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	{
		_orientation = new_orientation;
	}

	// rotates left and right
	_orientation = glm::rotate(_orientation, glm::radians(-rot_y), _up);

	// reset mouse position to the center
	glfwSetCursorPos(window, (_width / 2), (_height / 2));
}
glm::vec3 Camera::get_pos() const
{
	return _position;
}
