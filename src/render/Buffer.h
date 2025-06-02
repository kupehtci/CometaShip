#pragma once

#ifndef COMETA_BUFFER_H
#define COMETA_BUFFER_H

#include <cstdint>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "debug/Assertion.h"
#include "./DataType.h"



/**
* Buffer abstract class that define the minimal structure of a Renderer API Buffer
*/
class Buffer {
protected:
	uint32_t _uid = 0;

public:
	virtual ~Buffer() = default;
	/**
	* Bind the buffer to the active one
	*/
	virtual void Bind() = 0;
	virtual void Unbind() = 0;


	// --------- GETTERS ---------- // 
	[[nodiscard]] inline uint32_t GetUid() const { return _uid; }
};


/**
*	Vertex Buffer that contains vertices data
*/
class VertexBuffer : public Buffer{

private:

public:
	VertexBuffer();

	explicit VertexBuffer(uint32_t size);

	/**
	* Create a Vertex Buffer with the specified array of vertices and size
	* Data is defined so will be set to Static drawing because data won't change
	*/
	VertexBuffer(float* vertices, uint32_t size);
	~VertexBuffer() final;

	void Bind() override; 
	void Unbind() override;
};

/**
* Indexes Buffer that contains Indices information for Elements Buffers
*/
class IndexBuffer : public Buffer{

private:
	uint32_t _count = 0;

public:
	IndexBuffer(); 
	IndexBuffer(uint32_t* indices, uint32_t size);
	~IndexBuffer() final;

	void Bind() override;
	void Unbind() override;

	// ---------- GETTERS ---------- //
	[[nodiscard]] uint32_t GetCount() const { return _count;  }
};

#endif  //COMETA_BUFFER_H