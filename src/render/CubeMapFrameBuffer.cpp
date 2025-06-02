#include "CubeMapFrameBuffer.h"

CubeMapFrameBuffer::CubeMapFrameBuffer(uint32_t width, uint32_t height) : _width(width), _height(height) {
    // Create framebuffer
    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    
    // Create depth cubemap texture
    glGenTextures(1, &_depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _depthCubeMap);
    
    // Allocate storage for each face of the cubemap
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
                     width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    // Attach depth cubemap to framebuffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthCubeMap, 0);
    
    // We're only using the depth buffer, so tell OpenGL we don't need a color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    // Check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        COMETA_ERROR("Cubemap framebuffer is not complete!");
    }
    
    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CubeMapFrameBuffer::~CubeMapFrameBuffer() {
    Delete();
}

void CubeMapFrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glViewport(0, 0, _width, _height);
}

void CubeMapFrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CubeMapFrameBuffer::Delete() {
    if (_fbo != 0) {
        glDeleteFramebuffers(1, &_fbo);
        _fbo = 0;
    }
    
    if (_depthCubeMap != 0) {
        glDeleteTextures(1, &_depthCubeMap);
        _depthCubeMap = 0;
    }
}

void CubeMapFrameBuffer::BindDepthCubeMap(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _depthCubeMap);
}