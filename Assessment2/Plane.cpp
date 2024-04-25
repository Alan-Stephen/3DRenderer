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
	float t = fmod(glfwGetTime(), 15) / 15;
	glm::vec3 curr = _spline.lerp(t);

	glm::mat4 new_model = glm::translate(_model, curr);
	glm::vec3 next = _spline.get_direction(t);

	// rotate towards tangent of the spline, which can be done by "looking at" current position + tangent 
	glm::mat4 rotation = glm::inverse(glm::lookAt(curr,curr + next, glm::vec3(0.0, 1.0, 0.0)));
	new_model = new_model * rotation;

	// make the plane correctly orientated
	new_model = glm::rotate(new_model, glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f));
	return new_model;
}
