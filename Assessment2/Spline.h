#pragma once
#include <vector>
#include "glm/glm.hpp"
class Spline
{
public:
	Spline(std::vector<glm::vec3> control_points);
	glm::vec3 lerp(float t) const;
	glm::vec3 get_direction(float t) const;
private:
	std::vector<glm::vec3> _control_points;
};

