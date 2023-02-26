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



//Utility function for loading textures
unsigned int loadTexture(const char* path) {

	//Generate texture
	unsigned int texture;
	glGenTextures(1, &texture);

	//Load texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {

		GLenum format = GL_RED;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		//Generate texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Settings related to texture mapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
		stbi_image_free(data);
	}
	else {
		cout << "ERROR: Failed to load texture" << endl;
	}

	return texture;
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

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);

	//Initialize image loader
	stbi_set_flip_vertically_on_load(true);
	




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
	unsigned int diffuse_map = loadTexture("container2.jpg");
	unsigned int specular_map = loadTexture("container2_specular.jpg");
	unsigned int emission_map = loadTexture("container2_emission.png");

	lighting_shader.use();
	lighting_shader.setInt("material.diffuse", 0);
	lighting_shader.setInt("material.specular", 1);
	lighting_shader.setInt("material.emission", 2);



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

	//Define light and object color
	glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 object_color = glm::vec3(1.0f, 0.5f, 0.1f);
	glm::vec3 ambient_fac = glm::vec3(0.1f);
	glm::vec3 diffuse_fac = glm::vec3(0.75f);
	glm::vec3 specular_fac = glm::vec3(1.0f);



	//Used to translate light source over time
	float speed = 0.1f;
	float revolution_radius = 2.0f;
	glm::vec3 revolution_center = glm::vec3(0.0f, 1.0f, 0.0f);

	float object_rotation_speed = 10.0f;




	//Render loop
	while (!glfwWindowShouldClose(window)) {
		updateDeltaTime();

		//input
		processInput(window);

		//Update camera view
		view_matrix = glm::lookAt(camera.pos, camera.pos + camera.forward, camera.up);

		//Clear the color and depth buffers each frame
		glClearColor(0, 0, 0, 1.0f);
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



		//Render the object cube
		lighting_shader.use();

		lighting_shader.setVec3("light.position", light_cube_position);
		lighting_shader.setVec3("light.ambient", ambient_fac * light_color);
		lighting_shader.setVec3("light.diffuse", diffuse_fac * light_color);
		lighting_shader.setVec3("light.specular", specular_fac * light_color);

		lighting_shader.setVec3("view_pos", camera.pos);

		//lighting_shader.setVec3("material.specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
		lighting_shader.setFloat("material.sheen", 0.3f * 128);

		lighting_shader.setMat4("projection", projection_matrix);
		lighting_shader.setMat4("view", view_matrix);

		glm::mat4 cube_model_matrix = glm::mat4(1.0f);

		cube_model_matrix = glm::translate(cube_model_matrix, object_position);
		cube_model_matrix = glm::rotate(cube_model_matrix, (float) glm::radians(glfwGetTime() * object_rotation_speed), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));

		lighting_shader.setMat4("model", cube_model_matrix);

		//Calculate the normal matrix for the rotated model
		glm::mat3 normal_matrix = glm::mat3(glm::transpose(glm::inverse(cube_model_matrix)));
		lighting_shader.setMat3("normal_matrix", normal_matrix);


		//Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuse_map);

		//Bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specular_map);

		//Bind emission map
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emission_map);

		//Bind VAO
		glBindVertexArray(VAO);

		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 36);
		


		//Unbind VAO
		glBindVertexArray(0);
	
		//check & call events & swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Cleanup
	glfwTerminate();
	return 0;
}