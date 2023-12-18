#include "TextureLoader.h"


std::vector<ImageData> loadedImageData = std::vector<ImageData>();
std::vector<Texture> loaded_textures = std::vector<Texture>();


ImageData* LoadImageDataFromFile(std::string path, bool save, int desiredChannels) {
	for (unsigned int i = 0; i < loadedImageData.size(); i++) {
		if (loadedImageData[i].path == path)
			return &loadedImageData[i];
	}
	std::cout << "Loading NEW (Image Data): " << path << std::endl;

	//Load texture
	ImageData image = ImageData();
	image.data = stbi_load(path.c_str(), &image.width, &image.height, &image.nrChannels, desiredChannels);
	if (image.data) {

		GLenum format = GL_RED;
		if (image.nrChannels == 1)
			image.format = GL_RED;
		else if (image.nrChannels == 3)
			image.format = GL_RGB;
		else if (image.nrChannels == 4)
			image.format = GL_RGBA;

		stbi_image_free(image.data);
	}
	else {
		std::cout << "ERROR: Failed to load texture " << path << std::endl;
		return nullptr;
	}

	if (save)
		loadedImageData.push_back(image);

	return &image;

}



Texture* LoadTextureFromFile(std::string path, std::string texture_type) {

	//Return the texture if its already loaded
	for (unsigned int i = 0; i < loaded_textures.size(); i++) {
		if (loaded_textures[i].path == path) 
			return &loaded_textures[i];
	} 
	std::cout << "Loading NEW (Texture): " << path << std::endl;


	//Generate texture
	unsigned int texture;
	glGenTextures(1, &texture);

	//Load texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {

		GLenum format = GL_RED;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		//Generate texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Settings related to texture mapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "ERROR: Failed to load texture " << path << std::endl;
	}

	Texture loaded = Texture();
	loaded.id = texture;
	loaded.path = std::string(path);
	loaded.type = texture_type;
	loaded_textures.push_back(loaded);

	//return &loaded_textures[loaded_textures.size() - 1];
	return &loaded;

}