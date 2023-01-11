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
LockedCamera camera = LockedCamera();


//Setup variables to check time passing between each frame
float delta_time = 0.0f;
float last_frame_time = 0.0f;

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
	GLFWwindow* window = glfwCreateWindow(800, 600, "ooga booga", NULL, NULL);
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


	//Create shader program
	Shader shader("shader.vs", "shader.fs");

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);




	//Create Vertex Array & VBO
	float verticies[] = {
		//positions          //texels
		//front
		0.5f, 0.5f, -0.5f,   1.0f, 1.0f, //top right
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //bottom right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //bottom left
		-0.5f, 0.5f, -0.5f,  0.0f, 1.0f, //top left
		//back
		0.5f, 0.5f, 0.5f,    1.0f, 1.0f, //top right
		0.5f, -0.5f, 0.5f,   1.0f, 0.0f, //bottom right
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, //bottom left
		-0.5f, 0.5f, 0.5f,   0.0f, 1.0f, //top left
		//left
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		//right
		0.5f, 0.5f, 0.5f,    1.0f, 1.0f,
		0.5f, -0.5f, 0.5f,   1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, 0.5f, -0.5f,   0.0f, 1.0f,
		//top
		0.5f, 0.5f, 0.5f,    1.0f, 1.0f,
		0.5f, 0.5f, -0.5f,   1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		//bottom
		-0.5f, -0.5f, 0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, 0.5f,   0.0f, 1.0f
	};



	//Generate the VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO); 
	
	//Cube
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
	};



	stbi_set_flip_vertically_on_load(true);

	//Generate texture
	unsigned int texture0, texture1;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	//Settings related to texture mapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load texture
	int width0, height0, nrChannels0;
	unsigned char* data0 = stbi_load("container.jpg", &width0, &height0, &nrChannels0, 0);
	if (data0) {
		//Generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width0, height0, 0, GL_RGB, GL_UNSIGNED_BYTE, data0);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ERROR: Failed to load texture" << endl;
	}

	stbi_image_free(data0);



	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	int width1, height1, nrChannels1;
	unsigned char* data1 = stbi_load("rat.png", &width1, &height1, &nrChannels1, 0);
	if (data1) {
		//Generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "ERROR: Failed to load texture" << endl;
	}

	//Free image memory
	stbi_image_free(data1);

	



	//Generate the EBO (Describes the order to render the vertices)
	unsigned int EBO;
	glGenBuffers(1, &EBO);




	//Generate a VAO to describe how to handle the VBO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	//bind the VAO
	glBindVertexArray(VAO); 
	//copy the verticies array into a OpenGL Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); 
	//copy the indicies array into a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//Tell OpenGL how the vertex data is structured so it can send it to the vertex shader properly
	//params =   ( layout or attribute 'location', size of the vertex attribute, type of data, normalize?, stride = distance between each consecutive attribute in bytes, the offset )
	// - location data (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// - color data (r, g, b)
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	//glEnableVertexAttribArray(2);
	// - texel data (x, y);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);










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

	//Get transformation matrices' uniform locations from the vertex shader and apply them as appropriate
	unsigned int model_matrix_uniform_location = glGetUniformLocation(shader.ID, "model_matrix");
	unsigned int view_matrix_uniform_location = glGetUniformLocation(shader.ID, "view_matrix");
	unsigned int projection_matrix_uniform_location = glGetUniformLocation(shader.ID, "projection_matrix");
	glUniformMatrix4fv(model_matrix_uniform_location, 1, GL_FALSE, glm::value_ptr(model_matrix));
	glUniformMatrix4fv(view_matrix_uniform_location, 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniformMatrix4fv(projection_matrix_uniform_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));


	//Setup continuous rotation (will rotate objects as time passes)
	float rotation = 0;
	glm::mat4 rotated_model_matrix;

	int startms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	int nowms = 0;

	float beats_per_sec = 138.0f / 60.0f;
	float ms_per_rotation = 1000.0f / 360.0f;
	float rotate_time = (ms_per_rotation / beats_per_sec) * 16;



	//Define positions for cubes in the world
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f)
		/*glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)*/
	};



	//Render loop
	while (!glfwWindowShouldClose(window)) {
		updateDeltaTime();

		//input
		processInput(window);

		//Update camera view
		view_matrix = glm::lookAt(camera.pos, camera.pos + camera.forward, camera.up);
		
		//Update model matrix based off of current rotation
		nowms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		rotation = (startms - nowms) / (rotate_time);
		rotated_model_matrix = glm::rotate(model_matrix, glm::radians(rotation), glm::vec3(0.5, 0.5, 0.707));

		//rendering commands
		//glClearColor(0, 0.1f, 0.4f, 1.0f);
		glClearColor(0, 0, 0, 1.0f);
		//Clear the color and depth buffers each frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		//shader.setInt("texture0", 0);
		shader.setInt("texture1", 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//Update transformation matrices
		//glUniformMatrix4fv(model_matrix_uniform_location, 1, GL_FALSE, glm::value_ptr(rotated_model_matrix));
		glUniformMatrix4fv(view_matrix_uniform_location, 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(projection_matrix_uniform_location, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		glBindVertexArray(VAO);

		for (glm::vec3 cube_position : cubePositions) {
			glm::mat4 per_model_matrix = glm::mat4(1.0f);
			//per_model_matrix = glm::rotate(per_model_matrix, glm::radians(rotation), glm::normalize(cube_position));
			per_model_matrix = glm::translate(per_model_matrix, cube_position);
			shader.setMat4("model_matrix", per_model_matrix);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
		
		glBindVertexArray(0);

		//check & call events & swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Cleanup
	glfwTerminate();
	return 0;
}