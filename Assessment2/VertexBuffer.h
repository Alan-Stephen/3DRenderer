#pragma once
#include "GL/gl3w.h"
#include <vector>

class VertexBuffer
{
private:
	unsigned int _id;
public:
	VertexBuffer(const std::vector<float> &data);
	~VertexBuffer();

	void bind() const;
};

