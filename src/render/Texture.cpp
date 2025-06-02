#include "Texture.h"


Texture::Texture() {
	_uid = 0;
	_path = "";
	_width = _height = 0;
	_channels = 0;
}

Texture::Texture(const std::string& filePath){

	_uid = 0;
	_path = filePath; 
	_width = _height = 0; 
	_channels = 0; 
	
	// Load image using STBI
	int width, height, nrChannels;
	const char* texturePath = filePath.c_str();
	stbi_uc* data = nullptr; 

	data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	// Check that channels in RGB are 3 or 4
	if (nrChannels != 3 && nrChannels != 4) {
		COMETA_ERROR("Texture format not supported"); 
		return;
	}

	if (data != nullptr) {
		_width = width;
		_height = height;
		_channels = nrChannels;

		glGenTextures(1, &_uid);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, _uid);

		GLenum format = _channels == 4 ? GL_RGBA : GL_RGB; 
		GLenum internalFormat = _channels == 4 ? GL_RGBA8 : GL_RGB8; 

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else {
		COMETA_ERROR("[TEUnable to load texture");
		return; 
	}

	// COMETA_MSG("Texture loaded: ", _path);
}


void Texture::Bind(unsigned int index) {

    // The GL_TEXTUREi with i being an slot of the textures available,
    // can be obtained using the 0 slot and plus the index because they are a sequence
	if (index >= 32){
		COMETA_ERROR("Texture binding index out of scope, limited from 0 to 31");
		return;
	}
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, _uid);
}

Texture::~Texture() {
	Delete(); 	
}


void Texture::Delete(){
	if (_uid != 0) {
		glDeleteTextures(1, &_uid);
		_uid = 0;
		_width = 0;
		_height = 0;
		_channels = 0;
		_path = "";
	}
}

bool const Texture::operator==(const Texture& other) {
	return (_uid == other._uid); 
}