#ifndef COMETA_FRAMEBUFFER_H
#define COMETA_FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "../debug/Assertion.h"

class FrameBuffer {
private:
    uint32_t _fbo;           // Framebuffer object ID
    uint32_t _depthTexture;  // Depth texture ID for shadow mapping
    uint32_t _width;         // Width of the framebuffer
    uint32_t _height;        // Height of the framebuffer
    
public:
    FrameBuffer(uint32_t width, uint32_t height);
    ~FrameBuffer();
    
    void Bind();
    void Unbind();
    void Delete();
    
    // Binds the depth texture to the specified texture unit
    void BindDepthTexture(GLenum textureUnit);
    
    // Getters
    [[nodiscard]] uint32_t GetDepthTextureID() const { return _depthTexture; }
    [[nodiscard]] uint32_t GetWidth() const { return _width; }
    [[nodiscard]] uint32_t GetHeight() const { return _height; }
};

#endif // COMETA_FRAMEBUFFER_H