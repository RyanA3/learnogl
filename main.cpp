#include <iostream>
#include <glad/glad.h>
#include <glfw-3.3.8/include/GLFW/glfw3.h>
#include "shader.h"

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
		//positions         //colors
		//0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f, //top right
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, //bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, //bottom left
		//-0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f  //top left
		0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f  //top
	};

	//Generate the VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO); 

	//Create a EBO (Element Buffer Object) to specify what order to render the unique vertices in
	unsigned int indices[] = {
		1, 0, 2 //first triangle
		//3, 1, 2  //second triangle
	};

	//Texture coordinates
	unsigned int texels[] = {
		0.0f
	};

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	//Render loop
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//rendering commands
		glClearColor(0, 0.1f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//check & call events & swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Cleanup
	glfwTerminate();
	return 0;
}