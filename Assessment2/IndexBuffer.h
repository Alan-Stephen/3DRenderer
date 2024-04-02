#pragma once
#include <vector>
class IndexBuffer
{
private:
	unsigned int _id;
	unsigned int _count;

public:
	IndexBuffer(const std::vector<unsigned int> data);
	~IndexBuffer();

	void bind() const;
	unsigned int getCount() const;
};

