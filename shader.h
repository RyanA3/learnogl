#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using std::ifstream;
using std::cout;
using std::endl;

class Shader {

public:
	//Shader program id
	unsigned int ID;  

	//Constructor reads and builds shaders from respective files
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	//Activate the shader
	void use();

	//Uniform functions for easy use
	void setBool(const string& name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setMat3(const std::string& name, glm::mat3 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec4(const std::string& name, glm::vec4 value) const;

};

#endif