#include "Boat.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

Boat::Boat(std::string filename, glm::vec3 scale, glm::vec3 translate) : Model(filename, scale, translate), _base_y(translate.y)
{
	// note these have to be the exact same as in water.vert, otherwise the results of get_height will be wrong

    // reverse the translatoin from the base class constructor
    _model = glm::scale(glm::mat4(1.0f), scale);

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

    for (float x = -7; x < 7; x += 1.0f) {
        for (float y = -7; y < 7; y += 1.0f) {
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
    _orientation = glm::vec3(0, 0, 1);
}

glm::mat4 Boat::get_model() const
{
    // pick out the max y of the waves in sample radius.
    // todo: maybe make them rotate?
    glm::mat4 res = _model;
    glm::vec3 movement = _position;

    movement.y = get_max_height(glfwGetTime(), _position);

    // look at matrix also applies translation as well as rotation
    glm::mat4 rotation = glm::inverse(glm::lookAt(movement, movement + _orientation, glm::vec3(0.0, 1.0, 0.0)));

    return res * rotation;
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

float Boat::get_max_height(double timeSecs, glm::vec3 position) const
{
    float max_y = FLT_MIN;

    for (const glm::vec3& sample : _samples) {
        glm::vec3 curr_position = position + sample;
        max_y = max(get_height(static_cast<float>(timeSecs), curr_position),max_y );
    }

    assert(max_y != FLT_MIN);

    return max_y;
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

