#pragma once

#include <vector>

#include "Buffer.h"
#include "render/LayoutBuffer.h"


class VertexArray : public Buffer {

private: 
	std::vector<std::shared_ptr<VertexBuffer>> _vertexBuffers;
	std::shared_ptr<IndexBuffer> _indexBuffer;

	LayoutBuffer _layoutBuffer;
public: 
	VertexArray();
	~VertexArray() override;

	void Bind() override;
	void Unbind() override;

	void AddVertexBuffer(VertexBuffer& vertexBuffer);
	void AddIndexBuffer(IndexBuffer& indexBuffer);

	// Take into account that size is in bytes
	void CreateVertexBuffer(float* vertices, uint32_t size);
	void CreateIndexBuffer(uint32_t* indices, uint32_t size);

	std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() { return _vertexBuffers; }
	std::shared_ptr<IndexBuffer>& GetIndexBuffer() { return _indexBuffer; }

	LayoutBuffer& GetLayoutBuffer() { return _layoutBuffer; }
	void SetLayoutBuffer(const LayoutBuffer& layoutBuffer) { _layoutBuffer = layoutBuffer; }
};