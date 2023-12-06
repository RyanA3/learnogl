#include <iostream>
#include <glad/glad.h>
#include <glfw-3.3.8/include/GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include "camera.h"
#include <assimp/aabb.h>
#include "model.h"
#include "Light.h"
#include "LightManager.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Terrain.h"

//Note to self, for later physics engine
// https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/5collisionresponse/Physics%20-%20Collision%20Response.pdf

//Setup camera
bool camSpectator = true;
SpectatorCamera camera = SpectatorCamera();


//Setup variables to check time passing between each frame
float delta_time = 0.0f;
float last_frame_time = 0.0f;

//Other global variables


//Updates the delta time based on current system time
void updateDeltaTime() {
	float current_frame_time = glfwGetTime();
	delta_time = current_frame_time - last_frame_time;
	last_frame_time = current_frame_time;
}


//Processes when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//Input Processing Function
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	//Call camera update
	camera.processKeyInput(window, delta_time);
}


//Cursor movement processing function
void processMouseMovement(GLFWwindow* window, double xpos, double ypos) {
	
	//Call camera update
	camera.processMouseInput(window, xpos, ypos);
}





int main() {

	//Initlilizes GLFW with OpenGL version 3.3 CORE profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creates a window and brings it to front
	GLFWwindow* window = glfwCreateWindow(1440, 960, "OGL Model Renderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window failed to initialize" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 100, 50);


	//Loads GLAD and ensures it loaded properly
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD failed to initialize" << std::endl;
		return -1;
	}


	//Set scale
	glViewport(0, 0, 1440, 960);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//Capture the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Set callback for cursor movement
	glfwSetCursorPosCallback(window, processMouseMovement);


	//Create shader programs
	Shader light_source_shader("light_source_shader.vs", "light_source_shader.fs");
	Shader lighting_shader("lighting_shader.vs", "lighting_shader.fs");
	lighting_shader.num_point_lights = 1;
	lighting_shader.num_spot_lights = 1;

	Shader terrainShader("terrain.vs", "terrain.fs");

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//Initialize image loader
	stbi_set_flip_vertically_on_load(true);
	


	//Initialize lighting scene
	LightManager light_scene = LightManager();
	PointLight point_light = PointLight(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f), 1.0f, 0.14f, 0.07f);
	light_scene.addPointLight(point_light);

	DirectionalLight dir_light = DirectionalLight(glm::vec3(0.1f, 0.05f, 0.0125f), glm::vec3(0.75f, 0.5f, 0.05f), glm::vec3(0.5f, 0.5f, 0.25f), glm::vec3(0.0f, -1.0f, 0.0f));
	light_scene.addDirLight(dir_light);

	SpotLight spot_light = SpotLight(glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.5f), camera.pos, camera.forward,
		1.0f, 0.07f, 0.017f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
	light_scene.addSpotLight(spot_light);

	lighting_shader.use();
	light_scene.uploadLights(lighting_shader);
	lighting_shader.setFloat("material.sheen", 0.3f * 128);



	//Load models into scene
	Scene scene = Scene();
	Model backpackModel = Model("resources/models/backpack/backpack.obj");
	SceneObject backpack = SceneObject(backpackModel, glm::vec3(3.0f, 0, 0));
	backpack.applyForce(glm::vec3(0.0f, -9.8f, 0.0f));
	//scene.addObject(backpack);

	Model cubeModel = Model("resources/models/cube/cube.obj");
	SceneObject lightCube = SceneObject(cubeModel);
	lightCube.setScale(0.2f);
	glm::vec3 light_color = glm::vec3(0, 1.0f, 0);

	Terrain terrain = Terrain();


	//Enable wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//Main loop
	while (!glfwWindowShouldClose(window)) {
		updateDeltaTime();

		//input
		processInput(window);

		scene.update(delta_time / 10.0f);
		//backpack.update(delta_time / 10);

		//Clear the color and depth buffers each frame
		glClearColor(0.35f, 0.375f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		
		//Render the light source cube
		light_source_shader.use();
		light_source_shader.setVec3("light_color", light_color);
		camera.uploadMatrices(light_source_shader);
		lightCube.draw(light_source_shader);

		
		//Render the objects
		lighting_shader.use();
		camera.uploadMatrices(lighting_shader);
		lighting_shader.setVec3("spot_lights[0].position", camera.pos);
		lighting_shader.setVec3("spot_lights[0].direction", camera.forward);

		scene.draw(lighting_shader);

		//Render terrain
		terrainShader.use();
		camera.uploadMatrices(terrainShader);
		terrain.draw(terrainShader);

		camera.postdrawUpdate();
	
		//check & call events & swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	//Cleanup
	glfwTerminate();
	return 0;
}