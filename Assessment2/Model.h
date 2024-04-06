#pragma once
#include <vector>
#include "MaterialContainer.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "glm/common.hpp"
#include <vector>
#include "Mesh.h"
#include "Material.h"
#include <string>


class Model
{
private:
	std::vector<Mesh> _meshes;
	std::vector<Material> _materials;
	glm::mat4 _model = glm::mat4(1.f);
public:
	Model(std::string filename, glm::mat4 model, glm::vec3 scale, glm::vec3 translate);
	~Model();
	void draw(const Shader &shader) const;
	glm::mat4 get_model() const;
};

