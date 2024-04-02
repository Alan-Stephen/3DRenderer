#include "IndexBuffer.h"
#include "GL/gl3w.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned int> data)
{
	_count = data.size();
	glGenBuffers(1, &_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), GL_STATIC_DRAW);

}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &_id);
}

void IndexBuffer::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

unsigned int IndexBuffer::getCount() const { return _count; }
