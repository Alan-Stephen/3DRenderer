#pragma once
#include "glm/glm.hpp"
#include "Material.h"
#include "Model.h"
#include "Terrain.h"
#include "glm/gtc/type_ptr.hpp"

class Trees : public Model
{
public:
	Trees(int height, int width, glm::vec3 scale, glm::vec3 translate);
	void draw(const Shader &shader) const override;
	glm::mat4 get_model() const override;
private:
	Material _mat;
	unsigned int _instances;
};

