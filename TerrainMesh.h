#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include "vector";
#include "TextureLoader.h"


class TerrainMesh {
private:
	int resolution = 32;
	float width = 5.0f;
	float ds = width / (float) resolution;
	float dss = width / (float) (resolution * resolution);
	bool shouldUpdateFeatures = true;

	glm::vec3 pos = glm::vec3(0.0f);
	bool shouldUpdateModel = true;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int VBO, VAO, EBO;

	Texture* texture;
	bool shouldUpdateTexture = true;

public:
	TerrainMesh();
	TerrainMesh(int resolution, float width);

	void setupMesh();
	void draw(Shader& shader);

};