#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "error.h"
#include "texture.h"
#include "Shader.h"

#include "VertexBuffer.h"
#include "Camera.h"
#include "Model.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

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
	std::cout << "COMPILED SHADERS\n";

	std::cout << "PARSING OBJECTS\n";
	Model p_model = Model("objs/floor/floor.obj", glm::mat4(1.0f), glm::vec3(16.0f, 1.f, 16.f), glm::vec3(00.f, 0.f, 00.f));
	Model model = Model("objs/worm/worm.obj", glm::mat4(1.0f), glm::vec3(.1f, 0.1f, 0.1f), glm::vec3(00.f, 00.f, 00.f));
	std::cout << "FINISHED PARSING\n";

	glm::vec3 light_pos = glm::vec3(0.f, 2.f, 0.f);
	glm::vec3 light_col = glm::vec3(1.f, 1.f, 1.f);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSwapInterval(0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	shader.bind();

	glUniform3fv(shader.getUniformLocation("light_pos"), 1, glm::value_ptr(light_pos));
	glUniform3fv(shader.getUniformLocation("light_col"), 1, glm::value_ptr(light_col));

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

		if (diff >= (1.0 / 30.0)) {
			std::string fps = std::to_string((1 / diff) * count);
			std::string frame_time = std::to_string((diff / count) * 1000);
			std::string title = fps + " / " + frame_time + " ms";
			glfwSetWindowTitle(window, title.c_str());
			prev_time = curr_time;
			count = 0;
		}

		processKeyboard(window);
		camera.handleInput(window);
		camera.bind(45, 0.01f, 1000.f, shader, "cameraMat");

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUniform3fv(shader.getUniformLocation("cam_pos"), 1, glm::value_ptr(camera.get_pos()));
		p_model.draw(shader);
		model.draw(shader);

		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();

		processKeyboard(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

