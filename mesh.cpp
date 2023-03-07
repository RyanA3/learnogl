#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::setupMesh() {

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	//Load vertices into VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//Load indices into EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Format the VAO
	// - location data (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	// - normals to vertices (x, y, z)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	// - texel data (x, y);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texel));
	glEnableVertexAttribArray(2);

	//Unbind the VAO
	glBindVertexArray(0);

}

void Mesh::draw(Shader& shader) {

	//Load Textures
	unsigned int num_diffuse_textures = 0;
	unsigned int num_specular_textures = 0;
	for (int i = 0; i < textures.size(); i++) {

		//Activate texture to bind
		glActiveTexture(GL_TEXTURE0 + i);

		//Find out which number texture this is and inc the running total of its respective type
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(num_diffuse_textures++);
		else if (name == "texture_specular")
			number = std::to_string(num_specular_textures++);

		//Set the texture uniform for this material & bind the texture to the value of the uniform
		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);

	}

	glActiveTexture(GL_TEXTURE0);

	//Bind & Draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

};