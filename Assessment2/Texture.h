#pragma once
#include "GL/gl3w.h"
#include <string>

struct RGBA {
	RGBA(float r, float g, float b, float a): r(r), g(g), b(b), a(a)
	{}
	float r;
	float g;
	float b;
	float a;
};

class Texture {
public:
	Texture();
	Texture(std::string filename, bool is_flat, RGBA flat_colour);
	void deinit();
	void bind(GLint slot) const;
private:
	GLuint _id;
	int _height;
	int _width;
};

GLuint CreateTexture(const char* filename, int &height, int &width, bool is_flat, RGBA flat_colour);
