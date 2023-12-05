#include "CoordinateCross.h"

CoordinateCross::CoordinateCross() {
	this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
	generateModelData();
};

CoordinateCross::CoordinateCross(glm::vec3 pos, float scale) {
	this->pos = pos;
	this->scale = scale;
	generateModelData();
};

void CoordinateCross::generateModelData() {

	float new_vertices[] = {
		//x-axis (red)
		-(scale * neg_base), 0.0f, 0.0f,   100.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		 (scale * pos_base), 0.0f, 0.0f,   254.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		 //y-axis (green)
		 0.0f, -(scale * neg_base), 0.0f,   0.0f, 100.0f, 0.0f,   0.0f, 0.0f,
		 0.0f,  (scale * pos_base), 0.0f,   0.0f, 254.0f, 0.0f,   0.0f, 0.0f,
		 //z-axis (blue)
		 0.0f, 0.0f, -(scale * neg_base),   0.0f, 0.0f, 100.0f,   0.0f, 0.0f,
		 0.0f, 0.0f,  (scale * pos_base),   0.0f, 0.0f, 254.0f,   0.0f, 0.0f
	};

	for (int i = 0; i < 48; i++) {
		vertices[i] = new_vertices[i];
	}

};

void CoordinateCross::generateVAO() {

	//Generate VBO
	glGenBuffers(1, &VBO);

	//Generate EBO
	glGenBuffers(1, &EBO);

	//Generate & Bind VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Load vertices into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Load indices into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Format the VAO
	// - location data (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// - color data (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// - texel data (x, y);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

};

unsigned int CoordinateCross::getVAO() {
	return VAO;
};

void CoordinateCross::draw(Shader& shader, glm::vec3 location) {

	shader.setBool("color_mode", true);

	//Bind VAO
	glBindVertexArray(VAO);

	//Generate translated matrix for the coordinate cross's model and update it
	glm::mat4 cross_matrix = glm::mat4(1.0f);
	cross_matrix = glm::translate(cross_matrix, location);
	shader.setMat4("model_matrix", cross_matrix);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);

	//Unbind VAO
	glBindVertexArray(0);

}