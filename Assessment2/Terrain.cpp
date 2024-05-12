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

                
                
                verts.emplace_back(glm::vec3(x1, perlin(x1, z1, _model), z1), glm::vec2(x1 / repeat, z1 / repeat), perlin_norm(x1, z1));
                verts.emplace_back(glm::vec3(x1, perlin(x1, z2, _model), z2), glm::vec2(x1 / repeat, z2 / repeat), perlin_norm(x1, z2));
                verts.emplace_back(glm::vec3(x2, perlin(x2, z1, _model), z1), glm::vec2(x2 / repeat, z1 / repeat), perlin_norm(x2, z1));
                                                                                                   
                verts.emplace_back(glm::vec3(x1, perlin(x1, z2, _model), z2), glm::vec2(x1 / repeat, z2 / repeat), perlin_norm(x1, z2));
                verts.emplace_back(glm::vec3(x2, perlin(x2, z2, _model), z2), glm::vec2(x2 / repeat, z2 / repeat), perlin_norm(x2, z2));
                verts.emplace_back(glm::vec3(x2, perlin(x2, z1, _model), z1), glm::vec2(x2 / repeat, z1 / repeat), perlin_norm(x2, z1));
        }
    }

    _meshes.emplace_back(verts, 0, 8);
}

glm::vec2 Terrain::rand_grad(int ix, int iy) {

    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; 

    // this is to generate a psuedorandom number.
    unsigned a = ix, b = iy;
    a *= 2145254621;
 
    b ^= a << s | a >> w - s;
    b *= 4927358917;
 
    a ^= b << s | b >> w - s;
    a *= 1131119371;

    float random = a * (3.14159265 / ~(~0u >> 1)); 
    
    glm::vec2 v;
    v.x = sin(random);
    v.y = cos(random);
 
    return v;
}
 
// computes dot product of distance to random gradient.
float Terrain::grid_grad(int ix, int iy, float x, float y) {
    glm::vec2 gradient = rand_grad(ix, iy);
 
    float dx = x - (float)ix;
    float dy = y - (float)iy;
 
    return glm::dot(glm::vec2(dx,dy),gradient);
}
 
float Terrain::lerp(float start, float end, float t)
{
    // cubic interporalation to make it smoother;
    return (end - start) * (3.0 - t * 2.0) * t * t + start;
}

float Terrain::perlin(float a, float b, glm::mat4 model)
{
    glm::vec4 worldSpacePos = model * glm::vec4(a, 0, b, 1.0f);
    
    // determine grid cell corner coordinates
    float x = static_cast<float>(worldSpacePos.x) / static_cast<float>(80);
    float y = static_cast<float>(worldSpacePos.z) / static_cast<float>(80);

    int x0 = (int)x;
    int y0 = (int)y;

    int x1 = x0 + 1;
    int y1 = y0 + 1;

    // weights for lerping
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // lerp for top corners
    float n0 = grid_grad(x0, y0, x, y);
    float n1 = grid_grad(x1, y0, x, y);
    float ix0 = lerp(n0, n1, sx);

    // lerp for bottom corners
    n0 = grid_grad(x0, y1, x, y);
    n1 = grid_grad(x1, y1, x, y);
    float ix1 = lerp(n0, n1, sx);

    // final lerp
    float value = lerp(ix0, ix1, sy);

    // increase the amplitude;
    return value * 75;
}

// calculates normal of a vertex sampling perlin at point x,y
glm::vec3 Terrain::perlin_norm(int x, int y)
{

    // this just calculates the derivtive with respect to x and y on the plane from first principles.
    float delta = 0.1;

    glm::vec3 dx = glm::vec3(delta, 0, 0);
    float centre = perlin(x, y, _model);
    dx.y = perlin(x + delta, y, _model) - centre;


    glm::vec3 dz = glm::vec3(0, 0, delta);
    dz.y = perlin(x , y + delta, _model) - centre;
    dx = dx / delta;
    dz = dz / delta;

    // normal is cross product of tangent and binormal
    glm::vec3 res = glm::normalize(glm::cross((dz), (dx)));
    return res;
}


