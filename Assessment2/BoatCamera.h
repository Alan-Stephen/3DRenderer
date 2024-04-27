#pragma once
#include "Camera.h"
#include "Boat.h"
class BoatCamera : public Camera
{
public:
	BoatCamera(int width, int height, glm::vec3 position, Boat &boat);
	void handleInput(GLFWwindow* window) override;
	glm::vec3 get_pos() const override;
	glm::mat4 get_camera_mat(float fov, float nearPlane, float farPlane) override;
	void bind(float fov, float nearPlane, float farPlane, Shader& shader, std::string uniform) const override;
private:
	Boat& _boat;
};

