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
public:
	Model(std::string filename);
	~Model();
	void draw(const Shader &shader);
};

