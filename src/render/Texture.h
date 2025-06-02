#ifndef COMETA_TEXTURE_H
#define COMETA_TEXTURE_H

#include <cstdint>
#include <string>

// #include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

// #define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../debug/Assertion.h"

class Texture {

private: 
	uint32_t _uid;			// Unique ID of the loaded texture. In OpenGL this uid references the texture
	uint32_t _width;		// Width of the loaded texture
	uint32_t _height;		// Height of the loaded texture
	uint8_t _channels;		
	std::string _path;		// Store the path of the loaded texture

public: 
	Texture(); 

	Texture(const std::string& filePath);

	~Texture();

	void Bind(GLenum textureNumber);

	void Delete(); 

	// Operator overload
	
	bool const operator==(const Texture& other);



public:

	[[nodiscard]] uint32_t GetUID() const { return _uid; }
	inline uint32_t GetWidth() { return _width; }
	inline uint32_t GetHeight() { return _height;  }
	inline glm::vec2 GetSize() { return glm::vec2(_width, _height); }
	inline std::string GetPath() { return _path; }
	inline bool IsLoaded() { return _uid != 0;  }
};

#endif