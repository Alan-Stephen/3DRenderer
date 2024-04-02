#pragma once
#include <string>

class Shader {
public:
	Shader(std::string vsFilename, std::string fsFilename);
	~Shader();
	unsigned int getId() const;
	void bind();
	int getUniformLocation(std::string uniformName);
private:
	unsigned int _id;
	unsigned int compileShader(const char* vsFilename, const char* fsFilename) const;
};

