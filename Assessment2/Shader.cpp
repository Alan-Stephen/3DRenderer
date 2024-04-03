#include "Shader.h"
#include "GL/gl3w.h"
#include <stdio.h>
#include "file.h"
#include <string>

Shader::Shader(std::string vsFilename, std::string fsFilename)
{
	_id = compileShader(vsFilename.c_str(), fsFilename.c_str());
}

Shader::~Shader()
{
	glDeleteProgram(_id);
}

unsigned int Shader::getId() const
{
	return _id;
}

void Shader::bind()
{
	glUseProgram(_id);
}

int Shader::getUniformLocation(std::string uniformName) const
{
	return glGetUniformLocation(_id, uniformName.c_str());
}

GLuint Shader::compileShader(const char* vsFilename, const char* fsFilename) const
{
	int success;
	char infoLog[512];

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	char* vertexShaderSource = read_file(vsFilename);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader Failed Compilation\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	char* fragmentShaderSource = read_file(fsFilename);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Fragment Shader Failed Compilation\n" << infoLog << std::endl;
	}

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(program, 512, NULL, infoLog);
		std::cout << "Shader Program Failed Link\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	free(vertexShaderSource);
	free(fragmentShaderSource);

	return program;
}
