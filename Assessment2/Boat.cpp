#include "Boat.h"

Boat::Boat(std::string filename, glm::mat4 model, glm::vec3 scale, glm::vec3 translate) : Model(filename, model, scale, translate) 
{
	// note these have to be the exact same as in water.vert, otherwise the results of get_height will be wrong

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

    for (float x = 0; x < 7; x += 0.5) {
        for (float y = 0; y < 7; y += 0.5) {
            _samples.emplace_back(x, 0, y);
        }
    }

	// brownian motion parameters
	_amplitude_brownian = 0.70;
	_frequency_brownian = 1.13;
	_speed_brownian = 1.1;
    _num_subwaves = 8;
    _position = glm::vec3(translate.x, translate.y, translate.z);
    _base_y = translate.y;
}

glm::mat4 Boat::get_model() const
{
    // pick out the max y of the waves in sample radius.
    // todo: maybe make them rotate?
    glm::vec3 movement = glm::vec3(0,0,0);
    float max_y = FLT_MIN;

    for (const glm::vec3& sample : _samples) {
        glm::vec3 curr_position = _position + sample;
        max_y = max(get_height(static_cast<float>(glfwGetTime()), curr_position),max_y );
    }

    movement.y = max_y;
    return glm::translate(_model, movement);
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

