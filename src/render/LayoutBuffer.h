#pragma once

#ifndef AURAGL_LAYOUTBUFFER_H
#define AURAGL_LAYOUTBUFFER_H

#include <cstdint>
#include <vector>

#include "debug/Assertion.h"
#include "./DataType.h"

struct Layout { 
	std::string _name;		// debug name of the layout
	uint32_t _position;		// Position in the layout Renderer buffer
	DataType _type;			// Type of the data stored in the buffer
	uint32_t _size;			// Size of the data within the buffer
	uint32_t _offset;		// Offset of the layout's data from the start of each stride

	/**
	*  Default constructor for layout
	*/
	Layout() = default;
	
	/**
	*  Complete constructor by defining minimal structure
	*  Intended to calculate offset next
	*/
	Layout(const uint32_t position, const DataType type, const std::string& name) :
		_position(position), _type(type),  _name(name), 
		_offset(0)
	{
		_size = DataTypeCalculateSize(type); 
	}
};


class LayoutBuffer {

private:
	std::vector<Layout> _layouts;		// Array of layouts that compose the structure of the LayoutBuffer
	uint32_t _size = 0;					// Total stride of the LayoutBuffer

public: 
	/**
	*	Default constructor for LayoutBuffer
	*/
	LayoutBuffer() = default;

	/**
	* Struct similar constructor for the LayooutBuffer
	* Allows to create a LayoutBuffer like this: 
	*		LayoutBuffer layBuffer = {{0, DataType::Float3, "_position"}, {1, DataType::Float3, "_color"}}; 
	* 
	* In a format like a struct declaration
	* Once the layout buffer is set, it gets build to calculate each Layout's offset and size
	*/
	LayoutBuffer(std::initializer_list<Layout> layouts) : _layouts(layouts), _size(0)
	{
		Build();
	};

	/**
	*	Default destructor of LayoutBuffer
	*	Unbind all layout before destroy
	*/
	~LayoutBuffer(); 

	/**
	*   Build the layout buffer by calculating the stride and offset of each layout
	*/
	void Build(); 


	/**
	*	Enable the LayoutBuffer by enabling each layout
	*/
	void Enable();

	/**
	 * Add a new layout to the LayoutBuffer and re-build it
	 * @param layout new Layout to add
	 */
	void Add(const Layout& layout);

	/**
	*	Enable the LayoutBuffer by enabling each layout
	*/
	void Bind();

	/**
	*	Disable the LayoutBuffer by disabling each layout
	*/
	void Unbind();

	/**
	* Debug function that print by console the properties of each layout within the LayoutBuffer
	*/
	void Debug() const;

	[[nodiscard]] std::string ToString() const;

	// --------- GETTERS AND SETTERS ---------
	uint32_t GetSize() const { return _size; }
};

#endif // AURAGL_LAYOUTBUFFER_H