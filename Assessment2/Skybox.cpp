#include "Skybox.h"
#include "stb_image.h"

Skybox::Skybox(const Shader &shader)
{
	float skybox_verticies[] =
	{
		// verticies to be indexed to create a cube.
		-1.f, -1.f,  1.f,
		 1.f, -1.f,  1.f,
		 1.f, -1.f, -1.f,
		-1.f, -1.f, -1.f,
		-1.f,  1.f,  1.f,
		 1.f,  1.f,  1.f,
		 1.f,  1.f, -1.f,
		-1.f,  1.f, -1.f
	};

	// use indexes because they're easier to use.
	unsigned int skybox_indicies[] =
	{
		6, 2, 1, 1, 5, 6,
		7, 4, 0, 0, 3, 7,
		6, 5, 4, 4, 7, 6,
		2, 3, 0, 0, 1, 2,
		5, 1, 0, 0, 4, 5,
		6, 7, 3, 3, 2, 6
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

	std::string faces_cubemap[6] =
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

	// prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// iterate through all sides of the texture and bind them.
	for (int i = 0; i < 6; i++)
	{
		int width, height, channels;
		unsigned char* data = stbi_load(faces_cubemap[i].c_str(), &width, &height, &channels, 0);
		if (data)
		{
			// prevent skybox texture from loading in upsidown.
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "[ERROR] failed to load texture: " << faces_cubemap[i] << std::endl;
			stbi_image_free(data);
			return;
		}
	}

}

void Skybox::draw(const Shader &shader, const glm::mat4 &camera_mat)
{
	// we want skybox to always render last.
	glDepthFunc(GL_LEQUAL);

	shader.bind();

	glUniformMatrix4fv(shader.get_uniform_location("camera_mat"), 1, GL_FALSE, glm::value_ptr(camera_mat));
	glBindVertexArray(_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// switch back to normal depth map to not break things.
	glDepthFunc(GL_LESS);
}

Skybox::~Skybox()
{
	glDeleteTextures(1, &_texture);
}
