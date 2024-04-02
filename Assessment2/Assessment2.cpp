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
	GLFWwindow* window = glfwCreateWindow(width, height, "Assessment 2", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebguMessageCallback, 0);

	Shader shader("textured.vert","textured.frag");
	std::cout << "COMPILED SHADERS\n";


	//SECTION A - EDIT THIS CODE TO TEST
	Test0 T0;
	Test1 T1;
	Test2 T2;
	Test* tester = &T2;
	std::cout << "PARSING OBJECTS\n";
	std::vector<OldObject> objs = tester->ParseObj();
	std::cout << "FINISHED PARSING\n";
	//SECTION A - EDIT THIS CODE TO TEST



	//SECTION B - SETTING UP TEXTURE AND VAO FOR EACH OBJECT. DO NOT EDIT THIS CODE 
	for (int i = 0; i < objs.size(); i++)
	{
		std::cout << "SETTING UP OBJECT : " << i << " \n";
		if (strcmp(objs[i].mtl.fil_name,"")) {
			objs[i].texture = Texture(objs[i].mtl.fil_name, false, RGBA(255,255,255,255));
		}
		else {
			vec3 diffuse = objs[i].mtl.defuse_color;
			diffuse.x *= 255;
			diffuse.y *= 255;
			diffuse.z *= 255;
			objs[i].texture = Texture(objs[i].mtl.fil_name, true, RGBA(diffuse.x,diffuse.y,diffuse.z,255));
			std::cout << objs[i].mtl.mtl_name << " : " << diffuse.x << " " << diffuse.y << " " << diffuse.z << std::endl;
		}


		glGenVertexArrays(1, &objs[i].VAO);
		glGenBuffers(1, &objs[i].VBO);

		glBindVertexArray(objs[i].VAO);
		glBindBuffer(GL_ARRAY_BUFFER, objs[i].VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (objs[i].tris.size() * 18), objs[i].tris.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	//SECTION B - SETTING UP TEXTURE AND VAO FOR EACH OBJECT. DO NOT EDIT THIS CODE 

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//

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


		glm::mat4 model = glm::mat4(1.f);
		tester->Model(&model);
		glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));		//SECTION C - TEXTURE BINDING, MODEL MATRIX AND DRAW CALL FOR EACH OBJECT. DO NOT EDIT THIS CODE 
		for (int i = 0; i < objs.size(); i++)
		{
			glBindVertexArray(objs[i].VAO);
			objs[i].texture.bind(0);
			
			glDrawArrays(GL_TRIANGLES, 0, (objs[i].tris.size() * 3));
		}

		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();

		processKeyboard(window);
	}

	glfwTerminate();


	return 0;
}

