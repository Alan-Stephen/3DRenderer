#version 330 core


#define NUM_SUBWAVES 8

layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 cameraMat;
uniform mat4 light_projection;

uniform float time;

out vec3 pos;
out vec3 norm;

struct SubWave {
    float amplitude;
    float frequency;
    float speed;
    vec2 direction;
};

//SubWave subwaves[NUM_SUBWAVES] = SubWave[](
//    SubWave(20.0, 2.0 / 25.0, 1.0, vec2(1.0, 1.0)), // SubWave 1
 //   SubWave(4.5, 2.0 / 10.0, 6.0, vec2(0.8, .8)), // SubWave 2
  //  SubWave(2.5, 2.0 / 5.0, 8.0, vec2(0.4, 0.7)) // SubWave 3
//);


// array of random dirs for fractal brownian motion
vec2 random_dirs[NUM_SUBWAVES]  = vec2[] (
    vec2(0.56,0.02),
    vec2(-0.8,0.88),
    vec2(-0.15,0.74),
    vec2(0.85,0.22),
    vec2(0.48,0.56),
    vec2(0.67,0.73),
    vec2(-0.32,-0.91),
    vec2(-0.98,0.52)
);

// brownian motion parameters
float amplitude_brownian = 0.85;
float freq_brownian = 1.13;
float speed_brownian = 1.1;

void main()
{
    // Transform vertex position to model space
    pos = vec3(model * vec4(aPos, 1.0f));

    // Calculate the vertical displacement based on a sine wave
    float ddx = 0.0f;
    float ddz = 0.0f;
    float prev_x = 0;
    float prev_y = 0;
    float k = 1.5;
   
    SubWave wave = SubWave(10.0,.05,1.0, vec2(0.0,0.0));
    for (int i = 0; i < NUM_SUBWAVES; ++i) {
            
        // reduce amplitude increase frequency and speed, set to new random direction
        wave = SubWave(wave.amplitude * amplitude_brownian, wave.frequency * freq_brownian, wave.speed * speed_brownian, random_dirs[i]);
        
        // Calculate x-coordinate of the wave and add domain warping 
        float x = dot(wave.direction, pos.xz + vec2(prev_x,prev_y)) * wave.frequency + time * wave.speed;
        
        // Calculate y-coordinate offset based on sine wave
        float y_offset = wave.amplitude * exp(sin(x) - 1);
        
        // Calculate cosine result
        float cos_res = cos(x);
        
        // Calculate partial derivatives
        prev_x = wave.frequency * wave.direction.x * wave.amplitude * exp(sin(x) - 1) * cos_res;
        prev_y = wave.frequency * wave.direction.y * wave.amplitude * exp(sin(x) - 1) * cos_res;

        ddx += prev_x;
        ddz += prev_y;
        
        // Apply the vertical displacement to the y-coordinate of the position
        pos.y += y_offset;
    }

    // it looks different to the formula in GPU gems because they use weird (x,z,y) format idk why
    norm = normalize(vec3(-ddx, 1, -ddz));
    // Transform the position to camera space and set gl_Position
    gl_Position = cameraMat * vec4(pos, 1f);
}
