#include "Plane.h"

Plane::Plane(std::string filename, glm::vec3 scale, glm::vec3 translate, Spline spline,
	int repeat_interval_seconds, float initial_rotation_degrees) :
	Model(filename, scale, translate), _spline(spline), _initial_rotation_degrees(initial_rotation_degrees),
	_repeat_interval_seconds(repeat_interval_seconds), _scale(scale)
{}

void Plane::set_spline(Spline spline)
{
	_spline = spline;
}

glm::mat4 Plane::get_model() const 
{
	float t = fmod(glfwGetTime(), _repeat_interval_seconds) / _repeat_interval_seconds;
	glm::vec3 curr = _spline.lerp(t);

	glm::mat4 new_model = glm::translate(glm::mat4(1.0f), curr);
	glm::vec3 next = _spline.get_direction(t);

	// rotate towards tangent of the spline, which can be done by "looking at" current position + tangent 
	glm::mat4 rotation = glm::inverse(glm::lookAt(curr,curr + next, glm::vec3(0.0, 1.0, 0.0)));
	new_model = new_model * rotation;

	// make the plane correctly orientated
	new_model = glm::rotate(new_model, glm::radians(_initial_rotation_degrees), glm::vec3(0.0f, 1.0f, 0.0f));
	return glm::scale(new_model,_scale);
}
