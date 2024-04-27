#pragma once
#include "Model.h"
#include "glm/glm.hpp"
#include "Spline.h"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include <cmath>
#include <iostream>
#include <glm/gtx/string_cast.hpp>


class Plane : public Model
{
private:
	Spline _spline;
	int _repeat_interval_seconds;
	float _initial_rotation_degrees;
public:
	Plane(std::string filename, glm::vec3 scale, glm::vec3 translate, Spline spline,
		int repeat_interval_seconds, float initial_rotation_degrees);
	void set_spline(Spline spline);
	glm::mat4 get_model() const override;
};

