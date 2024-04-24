#version 330 core


#define NUM_SUBWAVES 3

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

SubWave subwaves[NUM_SUBWAVES] = SubWave[](
    SubWave(20.0, 2.0 / 25.0, 2.0, vec2(1.0, 1.0)), // SubWave 1
    SubWave(4.5, 2.0 / 10.0, 4.0, vec2(0.8, .8)), // SubWave 2
    SubWave(1.5, 2.0 / 5.0, 8.0, vec2(0.4, 0.7)) // SubWave 3
);

void main()
{
    // Transform vertex position to model space
    pos = vec3(model * vec4(aPos, 1.0f));

    // Calculate the vertical displacement based on a sine wave
    float ddx = 0.0f;
    float ddz = 0.0f;
    float k = 1.5;
    for (int i = 0; i < NUM_SUBWAVES; ++i) {
        SubWave wave = subwaves[i];
        
        // Calculate x-coordinate of the wave
        float x = dot(wave.direction, pos.xz) * wave.frequency + time * wave.speed;
        
        // Calculate y-coordinate offset based on sine wave
        float y_offset = wave.amplitude * exp(sin(x) - 1);
        
        // Calculate cosine result
        float cos_res = cos(x);
        
        // Calculate partial derivatives
        ddx += wave.frequency * wave.direction.x * wave.amplitude * exp(sin(x) - 1) * cos_res;
        ddz += wave.frequency * wave.direction.y * wave.amplitude * exp(sin(x) - 1) * cos_res;
        
        // Apply the vertical displacement to the y-coordinate of the position
        pos.y += y_offset;
    }

    // it looks different to the formula in GPU gems because they use weird (x,z,y) format idk why
    norm = normalize(vec3(-ddx, 1, -ddz));
    // Transform the position to camera space and set gl_Position
    gl_Position = cameraMat * vec4(pos, 1f);
}
