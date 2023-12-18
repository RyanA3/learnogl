#pragma once
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glfw-3.3.8/include/GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>

/*
* Stores raw image data on CPU
*/
struct ImageData {

	std::string path;
	unsigned char* data;
	int width, height, nrChannels;
	GLenum format;

	//For 3 channel images
	glm::vec3 sample(float sx, float sy) {
		int x = width * sx;
		int y = width * sy;
		int index = x + (y * width);

		return glm::vec3(data[index], data[index+1], data[index+2]);
	}

};

/*
* Structure for storing data about a texture loaded on the GPU
*/
struct Texture {

	unsigned int id;
	std::string type;
	std::string path;

};

//Utility function for loading textures
Texture* LoadTextureFromFile(std::string path, std::string texture_type);