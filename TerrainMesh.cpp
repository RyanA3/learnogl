#include "TerrainMesh.h"

//https://www.youtube.com/watch?v=5zlfJW2VGLM

TerrainMesh::TerrainMesh(ImageData* heightmap, Texture* surfaceTexture, glm::vec2 heightmapBegin, glm::vec2 heightmapSpan, int resolution, float width, float height) : resolution(resolution), width(width), height(height) {
	//Generate vertices
	for (int row = 0; row < resolution; row++) {
		for (int col = 0; col < resolution; col++) {
			float x = (((float)row + 0.5f) / resolution) * heightmapSpan.x + heightmapBegin.x;
			float z = (((float)col + 0.5f) / resolution) * heightmapSpan.y + heightmapBegin.x;
			vertices.push_back((heightmap->sample(x, z).x / 255.0f) * height);
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

	this->texture = surfaceTexture;

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
		model = glm::translate(model, pos);
		shader.setMat4("model", model);
		shouldUpdateModel = false;
	}

	if (shouldUpdateFeatures) {
		shader.setInt("resolution", resolution);
		shader.setFloat("ds", ds);
		shouldUpdateFeatures = false;
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

}


float TerrainMesh::getHeight(glm::vec3 pos) {
	if (pos.x > this->pos.x + this->width) return 0;
	else if (pos.x < this->pos.x) return 0;
	if (pos.z > this->pos.z + this->width) return 0;
	else if (pos.z < this->pos.z) return 0;

	float sx = (pos.x - this->pos.x) * resolution / width;
	float sz = (pos.z - this->pos.z) * resolution / width;
	glm::vec2 samplePos = glm::vec2(sx, sz);

	//Indices of the vertices of the quad the sample position is in
	int vx = (int)(sx);
	int vz = (int)(sz);
	int i1 = vx * resolution + vz;
	int i2 = (vx + 1) * resolution + vz;
	int i3 = i1 + 1;
	int i4 = i2 + 1;
	//int i3 = vx * resolution + vz + 1;
	//int i4 = (vx + 1) * resolution + vz + 1;

	//Coordinates of the verticies of the quad the sample position is in
	glm::vec2 pos1, pos2, pos3, pos4;
	pos1 = glm::vec2(vx, vz);
	pos2 = glm::vec2(vx + 1, vz);
	pos3 = glm::vec2(vx, vz + 1);
	pos4 = glm::vec2(vx + 1, vz + 1);

	float d1 = glm::distance(samplePos, pos1);
	float d2 = glm::distance(samplePos, pos2);
	float d3 = glm::distance(samplePos, pos3);
	float d4 = glm::distance(samplePos, pos4);

	//If top left is closer than bottom right, the triangle is different but occupies the same square, 
	//the other two vertices stay the same regardless because they're shared by both triangles
	if (d4 < d1) {
		pos1 = pos4;
		d1 = d4;
		i1 = i4;
	}

	if (i1 >= vertices.size() || i2 >= vertices.size() || i3 >= vertices.size()) return 0;
	if (i1 < 0 || i2 < 0 || i3 < 0) return 0;

	float denom = 1 / (((pos2.y - pos3.y) * (pos1.x - pos3.x)) + ((pos3.x - pos2.x) * (pos1.y - pos3.y)));
	float w1 = (((pos2.y - pos3.y) * (samplePos.x - pos3.x)) + ((pos3.x - pos2.x) * (samplePos.y - pos3.y))) * denom;
	float w2 = (((pos3.y - pos1.y) * (samplePos.x - pos3.x)) + ((pos1.x - pos3.x) * (samplePos.y - pos3.y))) * denom;
	float w3 = 1 - w1 - w2;

	float h1 = w1 * vertices[i1];
	float h2 = w2 * vertices[i2];
	float h3 = w3 * vertices[i3];
	float h = (h1 + h2 + h3) / (w1 + w2 + w3);

	//std::cout << "Mesh Sample d1(" << w1 << ")" << " d2(" << w2 << ") d3(" << w3 << ")" << std::endl;

	return h;
}
