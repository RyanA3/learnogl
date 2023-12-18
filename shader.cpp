#include "shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	string vertexShaderCode;
	string fragmentShaderCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	//Ensure the file streams can throw exceptions
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		//Open the files
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		std::stringstream vShaderStream, fShaderStream;

		//Read each file's buffer contents into respective string streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//Close each file
		vShaderFile.close();
		fShaderFile.close();

		//Convert string streams into strings
		vertexShaderCode = vShaderStream.str();
		fragmentShaderCode = fShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_UNSUCCESSFUL_READ" << endl;
	}

	const char* vShaderCode = vertexShaderCode.c_str();
	const char* fShaderCode = fragmentShaderCode.c_str();


	//Compile the shaders (vertex & fragment)
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER); //Generate the shader
	glShaderSource(vertex, 1, &vShaderCode, NULL); //Attach source code
	glCompileShader(vertex); //Compile

	//Check if compilation succeeded
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER); //Generate the shader
	glShaderSource(fragment, 1, &fShaderCode, NULL); //Attach source code
	glCompileShader(fragment); //Compile

	//Check if compilation succeeded
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}


	//Create a shader program to link the two shaders together
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	//Check if the shader program linked properly
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}



	//Delete shader objects since they are now linked into the program and no longer individually needed
	glDeleteShader(vertex);
	glDeleteShader(fragment);
};



void Shader::use() {
	glUseProgram(ID);
};

void Shader::setBool(const string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
};

void Shader::setInt(const string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
};

void Shader::setTex(int location, int value) const {
	glUniform1i(location, value);
}

void Shader::setFloat(const string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
};

void Shader::setMat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
};

void Shader::setMat3(const std::string& name, glm::mat3 value) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
};

void Shader::setVec3(const std::string& name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
};

void Shader::setVec4(const std::string& name, glm::vec4 value) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}