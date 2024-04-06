#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position) : 
	_width(width), _height(height), _position(position)
{}

void Camera::bind(float fov, float nearPlane, float farPlane, Shader& shader, std::string uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(_position, _position + _orientation, _up);
	projection = glm::perspective(glm::radians(fov), (float)(_width / _height), nearPlane, farPlane);

	glUniformMatrix4fv(shader.get_uniform_location(uniform),1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::handleInput(GLFWwindow* window)
{
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
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		_position += _speed * -_up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		_speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		_speed = 0.1f;
	}


	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (_firstClick)
		{
			glfwSetCursorPos(window, (_width / 2), (_height / 2));
			_firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = _sensitivity * (float)(mouseY - (_height / 2)) / _height;
		float rotY = _sensitivity * (float)(mouseX - (_width / 2)) / _width;

		glm::vec3 newOrientation = glm::rotate(_orientation, glm::radians(-rotX), glm::normalize(glm::cross(_orientation, _up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, _up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			_orientation = newOrientation;
		}

		// Rotates the Orientation left and right
		_orientation = glm::rotate(_orientation, glm::radians(-rotY), _up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (_width / 2), (_height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		_firstClick = true;
	}

}
glm::vec3 Camera::get_pos() const
{
	return _position;
}
