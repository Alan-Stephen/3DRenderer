#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const std::vector<float> &data) {
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ARRAY_BUFFER, _id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size() * 18, data.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer(){
	glDeleteBuffers(1, &_id);
}

void VertexBuffer::bind() const{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

