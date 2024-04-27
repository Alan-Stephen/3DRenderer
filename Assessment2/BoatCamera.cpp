#include "BoatCamera.h"
#include <iostream>

BoatCamera::BoatCamera(int width, int height, glm::vec3 position, Boat& boat) : Camera(width, height, position), _boat(boat)
{
}

void BoatCamera::handleInput(GLFWwindow * window)
{
	std::cout << "processing input here\n";
	glm::vec3 position = _boat.get_position();
	glm::vec3 orientation = _boat.get_orientation();
	glm::vec3 orient_without_y = glm::vec3(_boat.get_orientation().x, 0.0f, _boat.get_orientation().z);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += _speed * orient_without_y;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{

		orientation = glm::rotate(orientation, glm::radians(-1.0f), _up);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += _speed * -orient_without_y;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		orientation = glm::rotate(orientation, glm::radians(1.0f), _up);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		_speed = 1.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		_speed = 1.1f;
	}
	
	_boat.set_orientation(orientation);
	_boat.set_position(position);
}

glm::vec3 BoatCamera::get_pos() const
{
	// position of y will be stale so we have to update y value ourselves
	glm::vec3 pos = _boat.get_position();
	pos.y = _boat.get_max_height(glfwGetTime(), pos);
	pos.y += 25;
	pos -= 60.0f * _boat.get_orientation();
	return pos;
}

glm::mat4 BoatCamera::get_camera_mat(float fov, float nearPlane, float farPlane)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	glm::vec3 pos = get_pos();

	view = glm::lookAt(pos, pos + _boat.get_orientation(), _up);
	projection = glm::perspective(glm::radians(fov), (float)(_width / _height), nearPlane, farPlane);

	return projection * view;

}

void BoatCamera::bind(float fov, float nearPlane, float farPlane, Shader& shader, std::string uniform) const
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	glm::vec3 pos = get_pos();

	glm::vec3 orient = _boat.get_orientation();
	orient.y = -0.1;
	view = glm::lookAt(pos, pos + orient, _up);
	projection = glm::perspective(glm::radians(fov), (float)(_width / _height), nearPlane, farPlane);

	glUniformMatrix4fv(shader.get_uniform_location(uniform),1, GL_FALSE, glm::value_ptr(projection * view));
}
