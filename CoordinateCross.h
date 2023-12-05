#pragma once

#include "shader.h"

/*
* Model for displaying an x-y-z cross,
* useful for keeping track of how the camera is oriented,
* especially for the LockedCamera.
*
* Can be repositioned to any location and scaled
*/
class CoordinateCross {

public:
	float vertices[48];
	unsigned int indices[6] = { 0, 1, 2, 3, 4, 5 };
	unsigned int VBO;
	unsigned int EBO;
	unsigned int VAO;

	glm::vec3 pos;
	float scale = 1.0f;
	float neg_base = 0.2f; //How far the coordinate axes go into the negative directions (modified by scale)
	float pos_base = 1.0f; //How far the coordinate axes go into the positive directions (modified by scale)

	CoordinateCross();
	CoordinateCross(glm::vec3 pos, float scale);

	/*
	* Generates the verticies to render the three(line) coordinate axes
	* In the format (for each axis)
	* row 1: x-start, y, z, r, g, b
	* row 2: x-end, y, z, r, g, b,
	* row 3: x, y-start, z, r, g, b,
	* ...
	*/
	void generateModelData();

	/*
	* Automagically generates a VAO for the current
	* VBO and vertex data
	*/
	void generateVAO();

	/*
	* Returns the OpenGL location for the VAO
	*/
	unsigned int getVAO();

	/*
	* Renders the cross
	*/
	void draw(Shader& shader, glm::vec3 location);

};