#include "Texture.h"
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Texture::Texture() {}
Texture::Texture(std::string filename, bool is_flat, RGBA flat_colour)
{
	_id = CreateTexture(filename.c_str(), _height, _width, is_flat, flat_colour);
	printf("texture created with id %d\n", _id);
}

void Texture::deinit()
{
	printf("texture at %d being decsonstructed\n", _id);
	glDeleteTextures(1, &_id);

}

void Texture::bind(GLint slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _id);
}

GLuint CreateTexture(const char* filename, int &height, int &width, bool is_flat, RGBA flat_colour)
{
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);



	unsigned char* pxls;
	int channels;
	if(!is_flat) {
		pxls = stbi_load(filename, &width, &height, &channels, 0);
	}
	else {
		channels = 4;
		width = 1;
		height = 1;
		pxls = new unsigned char[4];
		pxls[0] = static_cast<unsigned char>(flat_colour.r);
		pxls[1] = static_cast<unsigned char>(flat_colour.g);
		pxls[2] = static_cast<unsigned char>(flat_colour.b);
		pxls[3] = static_cast<unsigned char>(flat_colour.a);
	}


	if (pxls != NULL)
	{
		if (is_flat) {
			std::cout << "loaded flat colour" << std::endl;
		}
		else {
			printf("Loaded image with path : [%s]\n", filename);
		}
		
		if (channels == 4) {
			std::cout << "Texture had 4 channels\n";
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxls);
		}
		if (channels == 3) {
			std::cout << "Texture had 3 channels\n";
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
		}
	}
	else
	{
		printf("Failed to load image with path : [%s]\n", filename);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] pxls;

	glDisable(GL_BLEND);

	return texObject;
}
