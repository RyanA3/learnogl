#include <iostream>
#include <glad/glad.h>
#include <glfw-3.3.8/include/GLFW/glfw3.h>
#include "shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Processes when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//Input Processing Function
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}
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


	//Loads GLAD and ensures it loaded properly
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "GLAD failed to initialize" << std::endl;
		return -1;
	}


	//Set scale
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);





	//Create shader program
	Shader shader("shader.vs", "shader.fs");




	//Create Vertex Array & VBO
	float verticies[] = {
		//positions         //colors            //texels
		0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, //top right
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, //bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, //bottom left
		-0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  //top left
		//0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f  //top
	};

	//Generate the VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO); 

	//Create a EBO (Element Buffer Object) to specify what order to render the unique vertices in
	unsigned int indices[] = {
		1, 0, 2, //first triangle
		3, 2, 0  //second triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float rotation = 0.0f;
	glm::vec4 vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	unsigned int transUniformLocation = glGetUniformLocation(shader.ID, "transform");
	glUniformMatrix4fv(transUniformLocation, 1, GL_FALSE, glm::value_ptr(trans));



	//Render loop
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);
		rotation += 0.025f;
		glm::mat4 curTrans = glm::rotate(trans, glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0));

		//rendering commands
		//glClearColor(0, 0.1f, 0.4f, 1.0f);
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		//shader.setInt("texture0", 0);
		shader.setInt("texture1", 1);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO);
		glUniformMatrix4fv(transUniformLocation, 1, GL_FALSE, glm::value_ptr(curTrans));
		

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//check & call events & swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Cleanup
	glfwTerminate();
	return 0;
}