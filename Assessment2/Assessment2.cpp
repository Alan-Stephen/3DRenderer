#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include <memory>
#include "error.h"
#include "texture.h"
#include "Shader.h"

#include "VertexBuffer.h"
#include "Camera.h"
#include "Model.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Light.h"

#define NUM_POINT_LIGHTS 4
#define NUM_SPOT_LIGHTS 4
const int width = 1200;
const int height = 800;


void SizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}



void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void render_scene(std::vector<std::unique_ptr<Model>> &models, Camera &camera, Shader &shader, GLFWwindow *window) 
{
		camera.bind(45, 0.01f, 1000.f, shader, "cameraMat");

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUniform3fv(shader.get_uniform_location("cam_pos"), 1, glm::value_ptr(camera.get_pos()));

		for(int i = 0; i < models.size(); i++) {
			models.at(i).get()->draw(shader);
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);

}



int main(int argc, char** argv)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );  
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Coursework", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebguMessageCallback, 0);

	Shader shader("textured.vert", "textured.frag");
	Shader shadow_shader("shadow.vert", "shadow.frag");
	std::cout << "COMPILED SHADERS\n";

	std::cout << "PARSING OBJECTS\n";
	std::vector<std::unique_ptr<Model>> models;

	models.push_back(std::make_unique<Model>("objs/floor/floor.obj", glm::mat4(1.0f), glm::vec3(128.0f, 1.f, 128.f), glm::vec3(00.f, 0.f, 00.f)));
	models.push_back(std::make_unique<Model>("objs/white_oak/white_oak.obj", glm::mat4(1.0f), glm::vec3(.1f, 0.1f, 0.1f), glm::vec3(00.f, 00.f, 00.f)));
	std::cout << "FINISHED PARSING\n";

	shader.bind();

	// set up lighting in scene
	PointLight point_lights[NUM_POINT_LIGHTS];
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		point_lights[i] = PointLight(
			glm::vec3(0.0f + (i * 50), 10.f, 0.f + (i * 50)),
			glm::vec3(1.0f, 1.f, 1.f),
			glm::vec3(1.0f, 1.f, 1.f),
			glm::vec3(0.f, 0.f, 0.f),
			1.0f,
			0.1f,
			0.01f
		);

		point_lights[i].bind_at(i, "point_lights", shader);
	}

	SpotLight spot_lights[NUM_POINT_LIGHTS];
	for (int i = 0; i < NUM_SPOT_LIGHTS; i++) {
		spot_lights[i] = SpotLight(
			glm::vec3(-10.0f + (i * -50), 3.f, 0.f),
			glm::vec3(0.0f, 1.f, 0.f),
			glm::vec3(0.0f, 0.f, 0.f),
			glm::vec3(1.0f, 1.f, 1.f),
			glm::vec3(1.0f, 1.f, 1.f),
			0.95,
			0.90
		);

		spot_lights[i].bind_at(i, "spot_lights", shader);
	}

	DirectionalLight directional_light = DirectionalLight(glm::vec3(.0f,1.0f,.0f),
		glm::vec3(0.0f, 0.0f,0.0f),
		glm::vec3(.8, 0.8,0.75),
		glm::vec3(0.8f, 0.8f,0.75));

	directional_light.bind(shader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glEnable(GL_FRAMEBUFFER_SRGB);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	
	// enable shadows

	/*
	unsigned int shadow_map_fbo;
	glGenFramebuffers(1, &shadow_map_fbo);

	unsigned int shadow_width = 2028;
	unsigned int shadow_height = 2028;

	unsigned int shadow_map;
	glGenTextures(1, &shadow_map);
	glBindTexture(GL_TEXTURE_2D, shadow_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	*/


	Camera camera = Camera(width, height, glm::vec3(0.0, 0.0, 0.0));

	double prev_time = 0.0;
	double curr_time = 0.0;	
	double diff;
	unsigned int count = 0;
	while (!glfwWindowShouldClose(window))
	{

		// calculate fps
		curr_time = glfwGetTime();
		diff = curr_time - prev_time;
		count += 1;

		if (diff >= (1.0 / 2.0)) {
			std::string fps = std::to_string((1 / diff) * count);
			std::string frame_time = std::to_string((diff / count) * 1000);
			std::string title = fps + " / " + frame_time + " ms";
			glfwSetWindowTitle(window, title.c_str());
			prev_time = curr_time;
			count = 0;
		}

		processKeyboard(window);
		camera.handleInput(window);

		render_scene(models, camera, shader, window);

		glfwPollEvents();

		processKeyboard(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

