#include "TerrainMesh.h"

//https://www.youtube.com/watch?v=5zlfJW2VGLM

TerrainMesh::TerrainMesh() {
	//Generate vertices
	for (int row = 0; row < resolution; row++) {
		for (int col = 0; col < resolution; col++) {
			//vertices.push_back(0.0f);
			float x = (((row - ((float)resolution / 2)) / resolution)) * 2;
			float z = (((col - ((float)resolution / 2)) / resolution)) * 2;
			vertices.push_back(x * x * z);
		}
	}

	int rows = resolution;
	int cols = resolution;

	for (int row = 0; row < rows-1; row++) {
		indices.push_back(row * cols);

		for (int col = 0; col < cols; col++) {
			indices.push_back((row * cols) + col);
			indices.push_back(((row + 1) * cols) + col);
		}

		indices.push_back((row + 1) * cols + (cols - 1));
	}	

	setupMesh();

	this->texture = LoadTextureFromFile("C:\\Users\\Ryan\\source\\repos\\oglproject\\oglproject\\resources\\images\\container.jpg", "texture_diffuse");
	this->heightmap = LoadTextureFromFile("C:\\Users\\Ryan\\source\\repos\\oglproject\\oglproject\\resources\\images\\heightmap1.png", "texture_heightmap");
}

TerrainMesh::TerrainMesh(ImageData* heightmap, glm::vec2 heightmapBegin, glm::vec2 heightmapSpan, int resolution, float width) : resolution(resolution), width(width) {
	//Generate vertices
	for (int row = 0; row < resolution; row++) {
		for (int col = 0; col < resolution; col++) {
			//vertices.push_back(0.0f);
			float x = (float) row / resolution;
			float z = (float) col / resolution;
			vertices.push_back(heightmap->sample(x, z).x / 255.0f);
		}
	}

	int rows = resolution;
	int cols = resolution;

	for (int row = 0; row < rows - 1; row++) {
		indices.push_back(row * cols);

		for (int col = 0; col < cols; col++) {
			indices.push_back((row * cols) + col);
			indices.push_back(((row + 1) * cols) + col);
		}

		indices.push_back((row + 1) * cols + (cols - 1));
	}

	setupMesh();

	this->texture = LoadTextureFromFile("resources/images/container.jpg", "texture_diffuse");
	this->heightmap = LoadTextureFromFile("resources/images/heightmap1.png", "texture_heightmap");
	this->texture = this->heightmap;

}

void TerrainMesh::setupMesh() {
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	//Load vertices into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//Load indices into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Format the VAO
	// - location data (height) -- indices provide x z data
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbind the VAO
	glBindVertexArray(0);
}

void TerrainMesh::draw(Shader& shader) {

	if (shouldUpdateTexture) {
		glActiveTexture(GL_TEXTURE0);
		shader.setInt("terrainTexture", 0);
		glBindTexture(GL_TEXTURE_2D, texture->id);

		//glActiveTexture(GL_TEXTURE1);
		//shader.setInt("heightmap", 1);
		//glBindTexture(GL_TEXTURE_2D, heightmap->id);

		glActiveTexture(GL_TEXTURE0);
		shouldUpdateTexture = false;
	}

	if (shouldUpdateModel) {
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, pos);
		shader.setMat4("model", model);
		shouldUpdateModel = false;
	}

	if (shouldUpdateFeatures) {
		shader.setInt("resolution", resolution);
		shader.setFloat("width", width);
		shader.setFloat("invResolution", 1.0f / resolution);
		shader.setFloat("invResolutionSquare", 1.0f / (resolution * resolution));
		shader.setFloat("ds", ds);
		shader.setFloat("dss", dss);
		shouldUpdateFeatures = false;
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}