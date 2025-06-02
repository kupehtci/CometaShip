#ifndef COMETA_CUBEMAPFRAMEBUFFER_H
#define COMETA_CUBEMAPFRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vector>

#include "../debug/Assertion.h"

class CubeMapFrameBuffer {
private:
    uint32_t _fbo;                // Framebuffer object ID
    uint32_t _depthCubeMap;       // Depth cubemap texture ID for point light shadow mapping
    uint32_t _width;              // Width of the framebuffer
    uint32_t _height;             // Height of the framebuffer
    
public:
    CubeMapFrameBuffer(uint32_t width, uint32_t height);
    ~CubeMapFrameBuffer();
    
    void Bind();
    void Unbind();
    void Delete();
    
    // Binds the depth cubemap texture to the specified texture unit
    void BindDepthCubeMap(GLenum textureUnit);
    
    // Getters
    [[nodiscard]] uint32_t GetDepthCubeMapID() const { return _depthCubeMap; }
    [[nodiscard]] uint32_t GetWidth() const { return _width; }
    [[nodiscard]] uint32_t GetHeight() const { return _height; }
};

#endif // COMETA_CUBEMAPFRAMEBUFFER_H