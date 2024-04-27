#pragma once
#include "Model.h"
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "glm/common.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

class Boat : public Model
{
public:
	Boat(std::string filename, glm::mat4 model, glm::vec3 scale, glm::vec3 translate);
	glm::mat4 get_model() const override;
	glm::vec3 get_position() const;
	glm::vec3 get_orientation() const;
	void set_orientation(glm::vec3 orientation);
	void set_position(glm::vec3 position);

	float get_max_height(double timeSecs, glm::vec3 position) const;
private:
	std::vector<glm::vec2> _random_dirs;
	float _speed_brownian;
	float _amplitude_brownian;
	float _frequency_brownian;
	int _num_subwaves;
	glm::vec3 _position;
	glm::vec3 _orientation;
	float _base_y;
	std::vector<glm::vec3> _samples;

	float get_height(float timeSecs, glm::vec3 position) const;
};

struct SubWave {
    float amplitude;
    float frequency;
    float speed;
    glm::vec2 direction;
};

