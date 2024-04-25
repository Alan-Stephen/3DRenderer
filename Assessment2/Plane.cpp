#include "Plane.h"

Plane::Plane(std::string filename, glm::mat4 model, glm::vec3 scale, glm::vec3 translate, Spline spline) : 
	Model(filename, model, scale, translate), _spline(spline)
{}

void Plane::set_spline(Spline spline)
{
	_spline = spline;
}

glm::mat4 Plane::get_model() const 
{
	float t = (sin(glfwGetTime()) + 1) / 2;
	glm::mat4 new_model = glm::translate(_model, _spline.lerp(t));
	std::cout << glm::to_string(new_model) << std::endl;
	return new_model;
}
