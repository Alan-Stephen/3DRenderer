#include "Skybox.h"
#include "stb_image.h"

Skybox::Skybox(const Shader &shader)
{
	float skybox_verticies[] =
	{
		//   Coordinates
		-1.f, -1.f,  1.f,//        7--------6
		 1.f, -1.f,  1.f,//       /|       /|
		 1.f, -1.f, -1.f,//      4--------5 |
		-1.f, -1.f, -1.f,//      | |      | |
		-1.f,  1.f,  1.f,//      | 3------|-2
		 1.f,  1.f,  1.f,//      |/       |/
		 1.f,  1.f, -1.f,//      --------1
		-1.f,  1.f, -1.f
	};

	unsigned int skybox_indicies[] =
	{
		// Right
		6, 2, 1,
		1, 5, 6,
		// Left
		7, 4, 0,
		0, 3, 7,
		// Top
		6, 5, 4,
		4, 7, 6,
		// Bottom
		2, 3, 0,
		0, 1, 2,
		// Back
		5, 1, 0,
		0, 4, 5,
		// Front
		6, 7, 3,
		3, 2, 6
	};

	shader.bind();

	unsigned int skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_verticies), &skybox_verticies, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skybox_indicies), &skybox_indicies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::string facesCubemap[6] =
	{
		"objs/skybox/right.jpg",
		"objs/skybox/left.jpg",
		"objs/skybox/top.jpg",
		"objs/skybox/bottom.jpg",
		"objs/skybox/front.jpg",
		"objs/skybox/back.jpg"
	};

	// Creates the cubemap texture object
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "[ERROR] failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
			return;
		}
	}

}

void Skybox::draw(const Shader &shader, const glm::mat4 &camera_mat)
{
	glDepthFunc(GL_LEQUAL);

	shader.bind();

	glUniformMatrix4fv(shader.get_uniform_location("camera_mat"), 1, GL_FALSE, glm::value_ptr(camera_mat));
	glBindVertexArray(_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}

Skybox::~Skybox()
{
	// todo : delete ebo , vao and vbo
	glDeleteTextures(1, &_texture);
}
