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
		int px = sx * width;
		int py = sy * this->height;
		std::cout << sy << " * " << this->height << " = " << py << std::endl;
		int index = (py * width + px) * nrChannels;
		if (index < 0) index = 0;
		if (index > (width) * (height) * nrChannels) index = (((width) * (height)) * nrChannels) - nrChannels;
		unsigned char r, g, b;
		r = data[index];
		g = data[index + 1];
		b = data[index + 2];
		std::cout << "Sample @(" << sx << "," << sy << ") = (" << px << "," << py << ") -> rgb(" << r << "," << g << "," << b << ")" << std::endl;
		return glm::vec3(r, g, b);
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


ImageData* LoadImageDataFromFile(std::string path, bool save, int desiredChannels);

//Utility function for loading textures
Texture* LoadTextureFromFile(std::string path, std::string texture_type);