#pragma once
#include <iostream>
#include <glad/glad.h>
#include <glfw-3.3.8/include/GLFW/glfw3.h>
#include "stb_image.h"
#include "mesh.h"

//Utility function for loading textures
Texture* LoadTextureFromFile(std::string path, std::string texture_type);