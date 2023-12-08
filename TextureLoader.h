#pragma once
#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glfw-3.3.8/include/GLFW/glfw3.h>
#include "stb_image.h"

/*
* Structure for storing data about a particular texture
*/
struct Texture {

	unsigned int id;
	std::string type;
	std::string path;

};

//Utility function for loading textures
Texture* LoadTextureFromFile(std::string path, std::string texture_type);