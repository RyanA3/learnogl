#pragma once
#include <glm/glm.hpp>
#include "shader.h"
#include "vector";


class Terrain {
private:
	int resolution = 16;
	float width = 10.0f;
	float ds = width / resolution;
	float dss = width / (resolution * resolution);
	bool shouldUpdateFeatures = true;

	glm::vec3 pos = glm::vec3(0.0f);
	bool shouldUpdateModel = true;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int VBO, VAO, EBO;

public:
	Terrain();
	Terrain(int resolution, float width);

	void setupMesh();
	void draw(Shader& shader);

};