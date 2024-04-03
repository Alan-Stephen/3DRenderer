#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "error.h"
#include "texture.h"
#include "obj.h"
#include "Shader.h"
#include "test.h"

#include "VertexBuffer.h"
#include "Camera.h"
#include "Model.h"


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
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(width, height, "Coursework", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebguMessageCallback, 0);

	Shader shader("textured.vert","textured.frag");
	std::cout << "COMPILED SHADERS\n";

	std::cout << "PARSING OBJECTS\n";
	Model p_model = Model("objs/white_oak/white_oak.obj");
	std::cout << "FINISHED PARSING\n";


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	shader.bind();

	Camera camera = Camera(width, height, glm::vec3(0.0,0.0,0.0));
	while (!glfwWindowShouldClose(window))
	{
		processKeyboard(window);
		camera.handleInput(window);
		camera.bind(45, 0.01f, 1000.f, shader, "cameraMat");

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(p_model.get_model()));	
		p_model.draw(shader);

		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();

		processKeyboard(window);
	}

	glfwTerminate();


	return 0;
}

