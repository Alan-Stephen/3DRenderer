#include "Boat.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"
#include "math.h"

Boat::Boat(std::string filename, glm::vec3 scale, glm::vec3 translate) : Model(filename, scale, translate), _base_y(translate.y)
{
	// note these have to be the exact same as in water.vert, otherwise the results of get_height will be wrong

    // reverse the translation and scale from the base class constructor, so we can apply matrix transforms in right order
    _model = glm::mat4(1.0f);

    _random_dirs = {
		glm::vec2(0.56,0.02),
		glm::vec2(-0.8,0.88),
		glm::vec2(-0.15,0.74),
		glm::vec2(0.85,0.22),
		glm::vec2(0.48,0.56),
		glm::vec2(0.67,0.73),
		glm::vec2(-0.32,-0.91),
		glm::vec2(-0.98,0.52)
	};

    for (float x = -5; x < 5; x += 2.0f) {
        for (float y = -5; y < 5; y += 2.0f) {
            _samples.emplace_back(x, 0, y);
        }
    }

	// brownian motion parameters, these are the exact same as the parameters from water.vert
    // this has to be done so the height sampling is accurate
	_amplitude_brownian = 0.70f;
	_frequency_brownian = 1.13f;
	_speed_brownian = 1.1f;
    _num_subwaves = 8;
    _position = translate;
    _scale = scale;
    _orientation = glm::vec3(0, 0, 1);
}

glm::mat4 Boat::get_model() const
{
    // pick out the max y of the waves in sample radius.
    // todo: maybe make them rotate?
    glm::mat4 res = _model;
    glm::vec3 movement = _position;

    movement.y = get_height(glfwGetTime(), _position) + 3.0f;
	float forward_y = get_height(glfwGetTime(), _position + 1.f * _orientation) + 3.0f;
    glm::vec3 forward = movement + _orientation;
    forward.y = forward_y;


    glm::vec3 right = glm::cross(_orientation, glm::vec3(0.0, 1.0, 0.0));
    right.y = get_height(glfwGetTime(), _position + right) + 3.0f;
    right.y -= movement.y;

    glm::vec3 up = glm::cross(forward - movement, right);

    glm::mat4 rotation = glm::inverse(glm::lookAt(movement, forward, -up));
    return glm::scale(res * rotation,_scale);
}

glm::vec3 Boat::get_orientation() const
{
    return _orientation;
}

void Boat::set_orientation(glm::vec3 orientation)
{
    _orientation = orientation;
}

void Boat::set_position(glm::vec3 position)
{
    _position = position;
}

glm::vec3 Boat::get_position() const
{
    return _position;
}

glm::vec3 Boat::get_boat_up(float time) const
{
    glm::vec3 movement = _position;
    movement.y = get_height(glfwGetTime(), _position) + 3.0f;
	float forward_y = get_height(glfwGetTime(), _position + 1.f * _orientation) + 3.0f;
    glm::vec3 forward = movement + _orientation;
    forward.y = forward_y;


    glm::vec3 right = glm::cross(_orientation, glm::vec3(0.0, 1.0, 0.0));
    right.y = get_height(glfwGetTime(), _position + right) + 3.0f;
    right.y -= movement.y;

    right = glm::normalize(right);

    // reverse negated otherwise up will be facing wrong way.
    return -glm::normalize(glm::cross(glm::normalize(forward - movement), right));
}

float Boat::get_height(float timeSecs, glm::vec3 pos) const
{
    float prev_x = 0;
    float prev_y = 0;
    float pos_y = _base_y;
    SubWave wave = { 25.0,.05,1.0, glm::vec2(0.0,0.0) };
    for (int i = 0; i < _num_subwaves; ++i) {
            
        // reduce amplitude increase frequency and speed, set to new random direction
        wave = {wave.amplitude * _amplitude_brownian, wave.frequency * _frequency_brownian, wave.speed * _speed_brownian, _random_dirs[i]};
        
        // Calculate x-coordinate of the wave and add domain warping 
        float x = glm::dot(wave.direction, glm::vec2(pos.x, pos.z) + glm::vec2(prev_x, prev_y)) * wave.frequency + timeSecs * wave.speed;
        
        // Calculate y-coordinate offset based on sine wave
        float y_offset = wave.amplitude * exp(sin(x) - 1);
        
        // Calculate cosine result
        float cos_res = cos(x);
        
        // Calculate partial derivatives
        prev_x = wave.frequency * wave.direction.x * wave.amplitude * exp(sin(x) - 1) * cos_res;
        prev_y = wave.frequency * wave.direction.y * wave.amplitude * exp(sin(x) - 1) * cos_res;

        // Apply the vertical displacement to the y-coordinate of the position
        pos_y += y_offset;
    }

    return pos_y;
}

