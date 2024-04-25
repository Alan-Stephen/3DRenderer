#include "Spline.h"

Spline::Spline(std::vector<glm::vec3> control_points)
{
	_control_points = control_points;
}

// t is mean't to be a value from 0 -> 1, which will give a lerped position along the curve
glm::vec3 Spline::lerp(float t) const
{
	assert(t >= 0 && t <= 1);
	t *= _control_points.size() - 1;

	int p0, p1, p2, p3;

	p1 = (int)t;
	p2 = (p1 + 1) % _control_points.size();
	p3 = (p2 + 1) % _control_points.size();
	p0 = p1 >= 1 ? p1 - 1 : _control_points.size() - 1;
	t = t - (int)t;

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -ttt + 2.0f*tt - t;
	float q2 = 3.0f*ttt - 5.0f*tt + 2.0f;
	float q3 = -3.0f*ttt + 4.0f*tt + t;
	float q4 = ttt - tt;

	float tx = 0.5f * (_control_points[p0].x * q1 + _control_points[p1].x * q2 + _control_points[p2].x * q3 + _control_points[p3].x * q4);
	float ty = 0.5f * (_control_points[p0].y * q1 + _control_points[p1].y * q2 + _control_points[p2].y * q3 + _control_points[p3].y * q4);
	float tz = 0.5f * (_control_points[p0].z * q1 + _control_points[p1].z * q2 + _control_points[p2].z * q3 + _control_points[p3].z * q4);

	return glm::vec3(tx, ty, tz);
}

glm::vec3 Spline::get_direction(float t) const
{
	t *= _control_points.size();

	int p0, p1, p2, p3;

	p1 = (int)t;
	p2 = (p1 + 1) % _control_points.size();
	p3 = (p2 + 1) % _control_points.size();
	p0 = p1 >= 1 ? p1 - 1 : _control_points.size() - 1;
	t = t - (int)t;

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -3.0f * tt + 4.0f*t - 1;
	float q2 = 9.0f*tt - 10.0f*t;
	float q3 = -9.0f*tt + 8.0f*t + 1.0f;
	float q4 = 3.0f*tt - 2.0f*t;

	float tx = 0.5f * (_control_points[p0].x * q1 + _control_points[p1].x * q2 + _control_points[p2].x * q3 + _control_points[p3].x * q4);
	float ty = 0.5f * (_control_points[p0].y * q1 + _control_points[p1].y * q2 + _control_points[p2].y * q3 + _control_points[p3].y * q4);
	float tz = 0.5f * (_control_points[p0].z * q1 + _control_points[p1].z * q2 + _control_points[p2].z * q3 + _control_points[p3].z * q4);

	return glm::vec3(tx, ty, tz);
}
