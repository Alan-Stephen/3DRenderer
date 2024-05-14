#pragma once
#include <vector>
#include "Shader.h"
#include "glm/common.hpp"
#include <vector>
#include "Mesh.h"
#include "Material.h"
#include <string>


class Model
{
protected:
	std::vector<Mesh> _meshes;
	std::vector<Material> _materials;
	glm::mat4 _model = glm::mat4(1.f);
	Model() {};
public:
	Model(std::string filename, glm::vec3 scale, glm::vec3 translate);
	Model(std::string filename, glm::vec3 scale, glm::vec3 translate, unsigned stride);
	~Model();
	virtual void draw(const Shader &shader) const;
	virtual glm::mat4 get_model() const;
};

