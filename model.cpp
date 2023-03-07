#include "model.h"



Model::Model(std::string path) {
	loadModel(path);
}

void Model::draw(Shader& shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}


void Model::loadModel(std::string path) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//Ensure scene loaded properly by checking if the scene or root node is null
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::INCOMPLETE_MODEL_LOAD " << importer.GetErrorString();
		return;
	}

	//Get the directory/folder that the model's file is in
	this->directory = path.substr(0, path.find_last_of('/'));

	//Call the first node process on the root node to recursively process all its children
	processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene) {

	//Process all the meshes pertaining to this particular node (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//Call node process on all child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//Process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex = Vertex();

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		//Process texture coords (if the mesh has any)
		if (mesh->mTextureCoords[0]) {
			vertex.texel.x = mesh->mTextureCoords[0][i].x;
			vertex.texel.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.texel = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	//Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);

	}

	//Process textures (if any)
	if (mesh->mMaterialIndex >= 0) {

		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<Texture> specular_maps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

		std::vector<Texture> emission_maps = loadMaterialTextures(mat, aiTextureType_EMISSIVE, "texture_emission");
		textures.insert(textures.end(), emission_maps.begin(), emission_maps.end());

	}

	return Mesh(vertices, indices, textures);

}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name) {

	std::vector<Texture> textures;
	unsigned int num_textures = mat->GetTextureCount(type);

	for (unsigned int i = 0; i < num_textures; i++) {

		//Gets the filepath of this texture
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = directory + '/' + std::string(str.C_Str());

		//Loads the texture into gpu memory using resource manager
		Texture* texture = LoadTextureFromFile(path, type_name);
		textures.push_back(*texture);

	}

	return textures;

}