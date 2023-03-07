#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "shader.h"

/*
* Structure for storing vertex data
*/
struct Vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texel;

};

/*
* Structure for storing data about a particular texture
*/
struct Texture {

	unsigned int id;
	std::string type;
	std::string path;

};


/*
* Class for storing data about a surface defined by
* 1. vertex data (vertices)
* 2. vertex rendering order (indices)
* 3. required textures to render (textures)
*/
class Mesh {

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	//Calls all gl draw calls to render the mesh
	void draw(Shader& shader);

private:
	unsigned int VBO, EBO, VAO;

	//Uploads all mesh data to the gpu
	void setupMesh();

};