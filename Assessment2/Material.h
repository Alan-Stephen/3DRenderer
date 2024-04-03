#pragma once
#include "glm/glm.hpp"
#include <string>
#include "Texture.h"

class Material {
private:
	Texture _ambient;
	Texture _specular;
	Texture _diffuse;
	float   _shininess;
	std::string _name;
public:
	Material(const Texture &ambient, const Texture &specular, const Texture &diffuse, float shininess, const std::string &name);
	void deinit();
	void bind() const;
	std::string get_name() const;
};
