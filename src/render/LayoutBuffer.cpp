#include "LayoutBuffer.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include <sstream>

LayoutBuffer::~LayoutBuffer() {
	Unbind(); 
	_layouts.clear(); 
	_size = 0; 
}

void LayoutBuffer::Build() {
	_size = 0;

	if (_layouts.empty()) {
		COMETA_ASSERT("Building an empty layout buffer");
		return;
	}

	uint32_t calcOffset = 0;
	
	// For each layout position, sum the stride and size
	//
	for (int i = 0; i < _layouts.size(); i++) {
		// _layouts[i]._position = i;		      // Positions are settled in the LayoutBuffer declaration
		_layouts[i]._offset = calcOffset;
		calcOffset += _layouts[i]._size; 
		_size += _layouts[i]._size; 
	}
}

void LayoutBuffer::Enable() {

	// Enable each layout in the LayoutBuffer depending on its type of data
	for (const Layout layout : _layouts) {
		switch (layout._type) {
		case DataType::Float:
		case DataType::Float2:
		case DataType::Float3:
		case DataType::Float4:
			glVertexAttribPointer(layout._position,
								static_cast<int>(DataTypeCalculateNumberElements(layout._type)),
								GL_FLOAT,
								GL_FALSE,
								static_cast<int>(_size),
								reinterpret_cast<void*>(layout._offset));
			glEnableVertexAttribArray(layout._position);
			break;

		default:
			std::cout << "not implemented yet mat3 and mat4 layout implementation" << std::endl;
			break;
		}

	}
}

void LayoutBuffer::Add(const Layout& layout)
{
	_layouts.push_back(layout);
	Build();
}


void LayoutBuffer::Bind() {
	Enable();
}

void LayoutBuffer::Unbind() {

	// Disable each layout
	for (const Layout layout : _layouts) {
		glDisableVertexAttribArray(layout._position); 
	}

	// _layouts.clear(); 

	_size = 0; 
}

void LayoutBuffer::Debug() const {
	for (auto const& layout : _layouts) {
		std::cout << "Layout: " << layout._position << " " << layout._name << " type: " << static_cast<int>(layout._type) << " with stride: " << layout._size << " and offset: " << layout._offset << std::endl;
	}
}

std::string LayoutBuffer::ToString() const
{
	std::stringstream stream;
	for (auto const& layout : _layouts) {
		stream << "Layout: " << layout._position
			   << "\t" << layout._name
			   << "\t type: " << static_cast<int>(layout._type)
			   << "\t stride: " << layout._size
			   << "\t offset: " << layout._offset
			   << "\n";
	}
	return stream.str();
}


