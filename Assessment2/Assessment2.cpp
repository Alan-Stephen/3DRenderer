#include <GL/gl3w.h>

#include <GLFW/glfw3.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include <memory>
#include "error.h"
#include "Shader.h"

#include "VertexBuffer.h"
#include "Camera.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Light.h"
#include "Water.h"
#include "Skybox.h"

#include "Model.h"
#include "Plane.h"
#include "Boat.h"
#include "BoatCamera.h"
#include "LightCube.h"
#include  "Terrain.h"
#include "Grass.h"

// no includes that use stb after stb_image_
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define NUM_POINT_LIGHTS 3
#define NUM_SPOT_LIGHTS 3
const int width = 1920;
const int height = 1080;
unsigned int shadow_width = 4048;
unsigned int shadow_height = 4048;


void SizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}



void processKeyboard(GLFWwindow* window, Camera *&camera, Camera &normal_camera, BoatCamera &boat_camera, bool &is_normal, double &time_last_called)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		if ((glfwGetTime() - time_last_called) > 0.1) {
			if (is_normal) {
				camera = &boat_camera;
				std::cout << "Switching to boat camera\n";
			}
			else {
				camera = &normal_camera;
				std::cout << "Switching to normal camera\n";
			}
			is_normal = !is_normal;
			time_last_called = glfwGetTime();
		}
	}
}

void set_up_shadow_map(Shader &shadow_shader, Shader &grass_shadow_shader,  unsigned int &shadow_map_fbo, unsigned int &shadow_map, glm::mat4 &light_projection) 
{
	glGenFramebuffers(1, &shadow_map_fbo);

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

	glm::mat4 orthogonal_projection = glm::ortho(-600.0f, 620.0f, -600.0f, 600.0f, 10.f, 2000.0f);
	glm::mat4 light_view = glm::lookAt(800.f * glm::vec3(.18, 0.42, 0.22), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.0f, 0.f));
	light_projection = orthogonal_projection * light_view;

	shadow_shader.bind();
	glUniformMatrix4fv(shadow_shader.get_uniform_location("light_projection"), 1, GL_FALSE, glm::value_ptr(light_projection));

	grass_shadow_shader.bind();
	glUniformMatrix4fv(grass_shadow_shader.get_uniform_location("light_projection"), 1, GL_FALSE, glm::value_ptr(light_projection));
}

void render_shadow_map(Shader &shadow_shader, unsigned int shadow_map_fbo, std::vector<std::unique_ptr<Model>> &models, const Boat &boat,
	const Grass &grass, const Shader &grass_shader) {
	shadow_shader.bind();
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, shadow_width, shadow_height);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_map_fbo);
	glClear(GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i < models.size(); i++) {
		models.at(i).get()->draw(shadow_shader);
	}	

	boat.draw(shadow_shader);

	// render with grass shadow shader.
	grass.draw(grass_shader);
}

void render_scene(std::vector<std::unique_ptr<Model>> &models, const Camera *camera, Shader &shader,
	glm::mat4 &light_projection, unsigned int shadow_map, const Boat &boat, const Grass &grass, Shader &grass_shader) 
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	shader.bind();

	glUniformMatrix4fv(shader.get_uniform_location("light_projection"), 1, GL_FALSE, glm::value_ptr(light_projection));
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, shadow_map);
	glUniform1i(shader.get_uniform_location("shadow_map"), 2);

	camera->bind(shader, "cameraMat");

	// todo move this into camera.bind
	glUniform3fv(shader.get_uniform_location("cam_pos"), 1, glm::value_ptr(camera->get_pos()));

	for(int i = 0; i < models.size(); i++) {
		models.at(i).get()->draw(shader);
	}

	boat.draw(shader);

	grass_shader.bind();
	
	glUniformMatrix4fv(grass_shader.get_uniform_location("light_projection"), 1, GL_FALSE, glm::value_ptr(light_projection));
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, shadow_map);
	glUniform1i(grass_shader.get_uniform_location("shadow_map"), 2);

	camera->bind(grass_shader, "cameraMat");

	grass.draw(grass_shader);

	glBindVertexArray(0);
}

void render_light_cubes(Shader& shader, Camera* camera, std::vector<std::unique_ptr<LightCube>> &point_light_cubes, std::vector<std::unique_ptr<LightCube>> &spot_light_cubes) {
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		point_light_cubes.at(i).get()->draw(shader, camera);
	}
	

	// not going to use spot lights as light cubes. 
	/*
	for (int i = 0; i < NUM_SPOT_LIGHTS; i++) {
		spot_light_cubes.at(i).get()->draw(shader, camera);
	}
	*/
}

int main(int argc, char** argv)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );  
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(width, height, "Coursework", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebguMessageCallback, 0);

	std::cout << "COMPILING SHADERS\n";
	Shader shader("textured.vert", "textured.frag");
	Shader shadow_shader("shadow.vert", "shadow.frag");
	Shader water_shader("water.vert", "textured.frag");
	Shader skybox_shader("skybox.vert", "skybox.frag");
	Shader light_cube_shader("light.vert", "light.frag");
	Shader grass_shader("grass.vert", "textured.frag");
	Shader grass_shadow_shader("grass_shadow.vert", "shadow.frag");

	std::cout << "COMPILED SHADERS\n";

	std::cout << "PARSING OBJECTS\n";
	std::vector<std::unique_ptr<Model>> models;

	Boat boat = Boat("objs/Boat/boat.obj", glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-50.f, 2.f, 150.f));
	models.push_back(std::make_unique<Model>("objs/house/house.obj", glm::vec3(30.0f, 30.f, 30.f), glm::vec3(00.f, 45.f, 00.f)));
	glm::mat4 model = glm::mat4(1.0f);

	std::vector<glm::vec3> control_points = {
		glm::vec3(150,100,150),
		glm::vec3(-150,100,150),
		glm::vec3(-150,100,-150),
		glm::vec3(150,100,-150),
		glm::vec3(150,100,150),
	};

	// plane should loop every 10 seconds, rotate it 180.0f intially, otherwise it'll be facing the wrong direction
	models.push_back(std::make_unique<Plane>("objs/birb/Jet_Lowpoly.obj", glm::vec3(10.f, 10.f, 10.f), glm::vec3(00.f, 0.f, 00.f), Spline(control_points), 10, 180.0f));

	models.push_back(std::make_unique<Terrain>(glm::vec3(4.0, 1.0, 4.0), glm::vec3(-400.0, 20.0, -400.0), 200, 200));

	//models.push_back(std::make_unique<Grass>(100, 100, glm::vec3(100.0, 100.0, 100.0), glm::vec3(100.0, 20.0, 100.0)200));

	Grass grass = Grass(780, 780, glm::vec3(20.0, 20.0, 20.0), glm::vec3(-390.0, 18.0, -390.0));

	Water water = Water(400,400, glm::vec3(1.5,1.5,1.5), glm::vec3(-200,0,-200));

	std::cout << "FINISHED PARSING\n";

	shader.bind();

	// set up lighting in scene
	PointLight point_lights[NUM_POINT_LIGHTS];
	std::vector<std::unique_ptr<LightCube>> point_light_cubes;

	point_lights[0] = PointLight(
		glm::vec3(21.0f, 65.f, 65.f),
		glm::vec3(1.0f, .1f, .1f),
		glm::vec3(1.0f, .1f, .1f),
		glm::vec3(0.f, 0.f, 0.f),
		.001f,
		.001f,
		0.001f
	);

	point_light_cubes.push_back(std::make_unique<LightCube>(glm::vec3(22.0f, 65.0f, 62.5f), glm::vec3(3.2,3.5,3.2), glm::vec3(1.0,.8,.8)));

	point_lights[0].bind_at(0, "point_lights", shader);
	point_lights[0].bind_at(0, "point_lights", water_shader);
	point_lights[0].bind_at(0, "point_lights", grass_shader);

	point_lights[1] = PointLight(
		glm::vec3(-21.0f, 68.f, 65.f),
		glm::vec3(1.0f, .1f, .1f),
		glm::vec3(1.0f, .1f, .1f),
		glm::vec3(0.f, 0.f, 0.f),
		.0001f,
		.001f,
		0.001f
	);

	point_light_cubes.push_back(std::make_unique<LightCube>(glm::vec3(-19.0f, 66.0f, 64.5f), glm::vec3(3.2,3.5,3.2), glm::vec3(1.0,.8,.8)));

	point_lights[1].bind_at(1, "point_lights", shader);
	point_lights[1].bind_at(1, "point_lights", water_shader);
	point_lights[1].bind_at(1, "point_lights", grass_shader);



	point_lights[2] = PointLight(
		glm::vec3(-170.0f, 65.0f, 135.5f),
		glm::vec3(1.0f, .1f, .1f),
		glm::vec3(1.0f, .1f, .1f),
		glm::vec3(0.f, 0.f, 0.f),
		.01f,
		.01f,
		0.01f
	);

	point_light_cubes.push_back(std::make_unique<LightCube>(glm::vec3(-185.0f, 65.0f, 144.5f), glm::vec3(3.2,3.5,3.2), glm::vec3(1.0,.8,.8)));

	point_lights[2].bind_at(2, "point_lights", shader);
	point_lights[2].bind_at(2, "point_lights", water_shader);
	point_lights[2].bind_at(2, "point_lights", grass_shader);

	// set up spot lights
	SpotLight spot_lights[NUM_SPOT_LIGHTS];
	std::vector<std::unique_ptr<LightCube>> spot_light_cubes;

	spot_lights[0] = SpotLight(
		glm::vec3(-3.0f, 94.f, -27.f),
		glm::vec3(0.0f, 1.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.f),
		glm::vec3(1.0f, 1.f, 1.f),
		glm::vec3(1.0f, 1.f, 1.f),
		0.98,
		0.90
	);

	spot_light_cubes.push_back(std::make_unique<LightCube>(glm::vec3(0, 60, 8), glm::vec3(1, 1, 1), glm::vec3(1.0,1.0,1.0)));
	spot_lights[0].bind_at(0, "spot_lights", shader);
	spot_lights[0].bind_at(0, "spot_lights", water_shader);
	spot_lights[0].bind_at(0, "spot_lights", grass_shader);

	spot_lights[1] = SpotLight(
		glm::vec3(-3.0f, 94.f, 24.f),
		glm::vec3(0.0f, 1.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.f),
		glm::vec3(1.0f, 1.f, 1.f),
		glm::vec3(1.0f, 1.f, 1.f),
		0.98,
		0.90
	);

	spot_light_cubes.push_back(std::make_unique<LightCube>(glm::vec3(-1, 60, -9), glm::vec3(1, 1, 1), glm::vec3(1.0,1.0,1.0)));
	spot_lights[1].bind_at(1, "spot_lights", shader);
	spot_lights[1].bind_at(1, "spot_lights", water_shader);
	spot_lights[1].bind_at(1, "spot_lights", grass_shader);


	spot_lights[2] = SpotLight(
		glm::vec3(-200.0f, 159.f, 101.f),
		glm::vec3(0.0f, 1.f, 0.f),
		glm::vec3(0.0f, 0.f, 0.f),
		glm::vec3(1.0f, .0f,1.0f),
		glm::vec3(1.0f, 1.f, 1.f),
		0.98,
		0.97
	);

	spot_light_cubes.push_back(std::make_unique<LightCube>(glm::vec3(-1, 60, -9), glm::vec3(1, 1, 1), glm::vec3(1.0,1.0,1.0)));
	spot_lights[2].bind_at(2, "spot_lights", shader);
	spot_lights[2].bind_at(2, "spot_lights", water_shader);
	spot_lights[2].bind_at(2, "spot_lights", grass_shader);

	DirectionalLight directional_light = DirectionalLight(glm::vec3(18,42.0f,21.1f),
		glm::vec3(0.2f, 0.2f,0.2f),
		glm::vec3(.5, .5,.5),
		glm::vec3(.5f, .5f,.5));

	directional_light.bind(shader);
	directional_light.bind(water_shader);
	directional_light.bind(grass_shader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	water_shader.bind();
	glUniform3fv(water_shader.get_uniform_location("light_pos"), 1, glm::value_ptr(glm::vec3(100.1f, 100.0f, 100.1f)));
	glUniform3fv(water_shader.get_uniform_location("light_col"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW); 
	glEnable(GL_MULTISAMPLE);

	
	unsigned int shadow_map_fbo;
	unsigned int shadow_map;
	glm::mat4 light_projection;
	// enable shadows
	set_up_shadow_map(shadow_shader, grass_shadow_shader, shadow_map_fbo, shadow_map, light_projection);
	
	//Skybox::init(skybox_shader);
	Skybox skybox = Skybox(skybox_shader);

	shader.bind();

	Camera normal_camera = Camera(width, height, glm::vec3(0.0, 0.0, 0.0), 45, 5, 3000);
	BoatCamera boat_camera = BoatCamera(width, height, glm::vec3(0.0, 0.0, 0.0), boat, 45, 1, 1000);

	Camera* camera = &normal_camera;
	bool is_normal = true;

	double prev_time = 0.0;
	double curr_time = 0.0;	
	double diff;
	unsigned int count = 0;
	double time_last_called = 0;
	std::cout << "==================== INSTRUCTIONS ======================== \n"; 
	std::cout << "hold the mouse button to pan camera around, release to free mouse\n";
	std::cout << "Press E to switch to boat camera.Move around in boat camera in WASD.\n";
	std::cout << "Press E to switch back to normal camera, WASD to movement and mouse to\n";
	std::cout << "look around. if you see black initally it's probably because you're in an object.\n";
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// calculate fps
		curr_time = glfwGetTime();
		diff = curr_time - prev_time;
		count += 1;

		if (diff >= (1.0 / 2.0)) {
			std::string fps = std::to_string((1 / diff) * count);
			std::string frame_time = std::to_string((diff / count) * 1000);
			std::string position = std::to_string(camera->get_pos().x) + "," +
				std::to_string(camera->get_pos().y) +
				"," + std::to_string(camera->get_pos().z);
			std::string title = fps + " / " + frame_time + " ms" + "/ position: " + position;
			glfwSetWindowTitle(window, title.c_str());
			prev_time = curr_time;
			count = 0;
		}

		processKeyboard(window, camera, normal_camera, boat_camera, is_normal, time_last_called);

		camera->handleInput(window);

		render_shadow_map(shadow_shader, shadow_map_fbo, models, boat, grass, grass_shadow_shader);

		render_scene(models, camera, shader, light_projection, shadow_map, boat, grass, grass_shader);


		skybox.draw(skybox_shader, camera->get_camera_mat());

		render_light_cubes(light_cube_shader, camera, point_light_cubes, spot_light_cubes);


		// IMPORTANT : draw water after everything because it's transparent!!!
		// not back face culling for water
		glDisable(GL_CULL_FACE);
		water.draw(water_shader, camera, light_projection, shadow_map);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW); 

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

