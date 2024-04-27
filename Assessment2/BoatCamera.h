#pragma once
#include "Camera.h"
#include "Boat.h"
class BoatCamera : public Camera
{
public:
	BoatCamera(int width, int height, glm::vec3 position, Boat &boat, float fov, float near_plane, float far_plane);
	void handleInput(GLFWwindow* window) override;
	glm::vec3 get_pos() const override;
	glm::mat4 get_camera_mat() override;
	void bind(Shader& shader, std::string uniform) const override;
private:
	Boat& _boat;
};

