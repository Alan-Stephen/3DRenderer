#include "Terrain.h"

Terrain::Terrain(glm::vec3 scale, glm::vec3 translate, int height, int width) :
    _height(height), _width(width)
{
	_model = glm::translate(glm::mat4(1.0f), translate);
	_model = glm::scale(_model, scale);

	Texture ambient = Texture("", true, RGBA(255.0, 255.0, 255.0, 255.0));
	Texture specular = Texture("objs/grass/normal.jpg", false, RGBA(255, 255, 255, 255));
	Texture diffuse = Texture("objs/grass/grass.jpg", false, RGBA(150.0, 150.0, 150.0, 255.0));
	_materials.emplace_back(ambient, specular, diffuse, 1, 1., "TERRAIN_MAT");
    float repeat = 32;


	std::vector<Vertex> verts;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
                float x1 = static_cast<float>(x);
                float z1 = static_cast<float>(y);
                float x2 = x1 + 1.0f;
                float z2 = z1 + 1.0f;

                
                
                verts.emplace_back(glm::vec3(x1, perlin(x1, z1), z1), glm::vec2(x1 / repeat, z1 / repeat), perlin_norm(x1, z1));
                verts.emplace_back(glm::vec3(x1, perlin(x1, z2), z2), glm::vec2(x1 / repeat, z2 / repeat), perlin_norm(x1, z2));
                verts.emplace_back(glm::vec3(x2, perlin(x2, z1), z1), glm::vec2(x2 / repeat, z1 / repeat), perlin_norm(x2, z1));
                                                                                                   
                verts.emplace_back(glm::vec3(x1, perlin(x1, z2), z2), glm::vec2(x1 / repeat, z2 / repeat), perlin_norm(x1, z2));
                verts.emplace_back(glm::vec3(x2, perlin(x2, z2), z2), glm::vec2(x2 / repeat, z2 / repeat), perlin_norm(x2, z2));
                verts.emplace_back(glm::vec3(x2, perlin(x2, z1), z1), glm::vec2(x2 / repeat, z1 / repeat), perlin_norm(x2, z1));
        }
    }

    _meshes.emplace_back(verts, 0);
}

typedef struct {
    float x, y;
} vector2;
 
vector2 randomGradient(int ix, int iy) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; 
    unsigned a = ix, b = iy;
    a *= 3284157443;
 
    b ^= a << s | a >> w - s;
    b *= 1911520717;
 
    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    
    // Create the vector from the angle
    vector2 v;
    v.x = sin(random);
    v.y = cos(random);
 
    return v;
}
 
// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    vector2 gradient = randomGradient(ix, iy);
 
    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;
 
    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}
 
float interpolate(float a0, float a1, float w)
{
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

float Terrain::perlin(float a, float b)
{
    glm::vec4 worldSpacePos = _model * glm::vec4(a, 0, b, 1.0f);
    
    // Determine grid cell corner coordinates
    float x = static_cast<float>(worldSpacePos.x) / static_cast<float>(_width / 2);
    float y = static_cast<float>(worldSpacePos.z) / static_cast<float>(_height / 2);

    int x0 = (int)x;
    int y0 = (int)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // Compute Interpolation weights
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Compute and interpolate top two corners
    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    // Compute and interpolate bottom two corners
    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sx);

    // Final step: interpolate between the two previously interpolated values, now in y
    float value = interpolate(ix0, ix1, sy);

    return value * 75;
}

glm::vec3 Terrain::perlin_norm(int x, int y)
{
    float delta = 0.1;

    glm::vec3 dx = glm::vec3(delta, 0, 0);
    float centre = perlin(x, y);
    dx.y = perlin(x + delta, y) - centre;


    glm::vec3 dz = glm::vec3(0, 0, delta);
    dz.y = perlin(x , y + delta) - centre;
    dx = dx / delta;
    dz = dz / delta;

    glm::vec3 res = glm::normalize(glm::cross((dz), (dx)));
    return res;
}


