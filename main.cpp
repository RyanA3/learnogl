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

//Setup camera
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
	GLFWwindow* window = glfwCreateWindow(800, 600, "Learnin Window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window failed to initialize" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 600, 200);


	//Loads GLAD and ensures it loaded properly
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD failed to initialize" << std::endl;
		return -1;
	}


	//Set scale
	glViewport(0, 0, 800, 600);
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

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//Initialize image loader
	stbi_set_flip_vertically_on_load(true);
	



	//Initialize lighting scene
	LightManager light_scene = LightManager();
	PointLight point_light = PointLight(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.75f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f), 1.0f, 0.14f, 0.07f);
	light_scene.addPointLight(point_light);

	DirectionalLight dir_light = DirectionalLight(glm::vec3(0.1f, 0.05f, 0.0125f), glm::vec3(0.75f, 0.5f, 0.05f), glm::vec3(0.5f, 0.5f, 0.25f), glm::vec3(0.0f, -1.0f, 0.0f));
	light_scene.addDirLight(dir_light);

	SpotLight spot_light = SpotLight(glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.5f), camera.pos, camera.forward,
		1.0f, 0.07f, 0.017f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));
	light_scene.addSpotLight(spot_light);

	lighting_shader.use();
	light_scene.uploadLights(lighting_shader);
	lighting_shader.setFloat("material.sheen", 0.3f * 128);


	//Cube vertices
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};


	
	//Cube
	/*
	unsigned int indices[] = {
		//front
		1, 0, 2,
		3, 2, 0,
		//back
		5, 4, 6,
		7, 6, 4,
		//left
		9, 8, 10,
		11, 10, 8,
		//right
		12, 15, 13,
		14, 13, 15,
		//top
		16, 18, 17,
		19, 18, 16,
		//bottom
		23, 22, 21,
		20, 23, 21
	};*/


	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	




	//Generate the VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//Generate a VAO to describe how to handle the VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	
	//Load vertices into buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	

	//Tell OpenGL how the vertex data is structured so it can send it to the vertex shader properly
	// - location data (x, y, z)
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	//Generate a VAO for rendering cube light
	unsigned int light_source_VAO;
	glGenVertexArrays(1, &light_source_VAO);
	glBindVertexArray(light_source_VAO);

	//Load vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Set attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	//Load textures
	Model backpack = Model("resources/models/backpack/backpack.obj");

	//lighting_shader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
	//lighting_shader.setFloat("light.outer_cutoff", glm::cos(glm::radians(15.0f)));


	//Generate translation matrices
	// - Generate model matrix to move object into world space
	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::rotate(model_matrix, glm::radians(-55.0f), glm::vec3(0.0, 1.0, 0.0));

	// - Generate view matrix to move scene relative to camera
	glm::mat4 view_matrix;
	view_matrix = glm::lookAt(camera.pos, camera.pos + camera.forward, camera.up);

	// - Generate projection matrix to map shit to the screen based on perspective
	//   Arg 1 = fov, arg 2 = aspect ratio, arg 3 = near plane, arg 4 = far plane
	glm::mat4 projection_matrix;
	projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);



	//Define positions for cubes in the world
	glm::vec3 object_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 light_cube_position = glm::vec3(1.0f, 2.0f, 1.0f);
	glm::vec4 light_position = glm::vec4(1.0f, 2.0f, 1.0f, 1.0f);
	glm::vec4 light_direction = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);


	//Used to translate light source over time
	float speed = 0.1f;
	float revolution_radius = 2.0f;
	glm::vec3 revolution_center = glm::vec3(0.0f, 1.0f, 0.0f);

	float object_rotation_speed = 10.0f;



	glm::vec3 light_color = glm::vec3(1.0f, 0.0f, 0.0f);


	//Render loop
	while (!glfwWindowShouldClose(window)) {
		updateDeltaTime();

		//input
		processInput(window);

		//Update camera view
		view_matrix = glm::lookAt(camera.pos, camera.pos + camera.forward, camera.up);

		//Clear the color and depth buffers each frame
		glClearColor(0.35f, 0.375f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		
		//Render the light source cube
		light_source_shader.use();
		light_source_shader.setVec3("light_color", light_color);
		light_source_shader.setMat4("view", view_matrix);
		light_source_shader.setMat4("projection", projection_matrix);

		glm::mat4 light_model_matrix = glm::mat4(1.0f);
		light_cube_position = revolution_center + (glm::vec3(sin(glfwGetTime() * speed), 0.0, cos(glfwGetTime() * speed)) * revolution_radius);
		light_model_matrix = glm::translate(light_model_matrix, light_cube_position);
		light_model_matrix = glm::scale(light_model_matrix, glm::vec3(0.2f));
		light_source_shader.setMat4("model", light_model_matrix);

		//Bind VAO
		glBindVertexArray(light_source_VAO);

		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 36);
		

		
		//Render the objects
		lighting_shader.use();

		lighting_shader.setVec3("view_pos", camera.pos);
		lighting_shader.setMat4("projection", projection_matrix);
		lighting_shader.setMat4("view", view_matrix);
		lighting_shader.setVec3("spot_lights[0].position", camera.pos);
		lighting_shader.setVec3("spot_lights[0].direction", camera.forward);

		glm::mat4 cube_model_matrix = glm::mat4(1.0f);
		cube_model_matrix = glm::rotate(cube_model_matrix, (float)glm::radians(glfwGetTime() * object_rotation_speed), glm::normalize(glm::vec3(2.0f, 1.0f, 3.0f)));

		lighting_shader.setMat4("model", cube_model_matrix);

		//Calculate the normal matrix for the rotated model
		glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(cube_model_matrix)));
		lighting_shader.setMat3("normal_matrix", normal_matrix);
		backpack.draw(lighting_shader);

	
		//check & call events & swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Cleanup
	glfwTerminate();
	return 0;
}