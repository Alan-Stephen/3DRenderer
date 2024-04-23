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
    SubWave(10.0, 2.0 / 25.0, 4.0, vec2(1.0, 0.5)), // SubWave 1
    SubWave(.5, 2.0 / 10.0, 6.0, vec2(1.0, 1.0)), // SubWave 2
    SubWave(0.1, 2.0 / 5.0, 10.0, vec2(0.0, 0.8)) // SubWave 3
);

void main()
{
    // Transform vertex position to model space
    pos = vec3(model * vec4(aPos, 1.0f));

    // Calculate the vertical displacement based on a sine wave
    float ddx = 0.0f;
    float ddz = 0.0f;
    for (int i = 0; i < NUM_SUBWAVES; ++i) {
        SubWave wave = subwaves[i];
        
        // Calculate x-coordinate of the wave
        float x = dot(wave.direction, pos.xz) * wave.frequency + time * wave.speed;
        
        // Calculate y-coordinate offset based on sine wave
        float y_offset = wave.amplitude * sin(x);
        
        // Calculate cosine result
        float cos_res = cos(x);
        
        // Calculate partial derivatives
        ddx += wave.frequency * wave.amplitude * wave.direction.x * cos_res;
        ddz += wave.frequency * wave.amplitude * wave.direction.y * cos_res;
        
        // Apply the vertical displacement to the y-coordinate of the position
        pos.y += y_offset;
    }

    norm = normalize(vec3(-ddx, -ddz, 1.0));
    // Transform the position to camera space and set gl_Position
    gl_Position = cameraMat * vec4(pos, 1f);
}
