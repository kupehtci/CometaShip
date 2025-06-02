
#ifndef COMETA_DATATYPE_H
#define COMETA_DATATYPE_H

// 
//	DataType abstraction from the OpenGL types of data to the Cometa's DataType class
// 

#include <glad/glad.h>
#include <GLFW/glfw3.h>


/**
*	Shader Data's types common in the Graphics API libraries
*	Helps to determine the type of data
*/
enum class DataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

/**
* Shader data's type's harcoded size.
* Returns Data type's size in bytes
*/
static uint32_t DataTypeCalculateSize(DataType type)
{
	switch (type)
	{
	case DataType::Float:
		return 4;
	case DataType::Float2:
		return 8;
	case DataType::Float3:
		return 12;
	case DataType::Float4:
		return 16;
	case DataType::Mat3:
		return 36;
	case DataType::Mat4:
		return 64;
	case DataType::Int:
		return 4;
	case DataType::Int2:
		return 8;
	case DataType::Int3:
		return 12;
	case DataType::Int4:
		return 16;
	case DataType::Bool:
		return 1;
	default:
		COMETA_WARNING("Unknown Data type for getting size");
		return 0;
	}
}

/**
 *	Get the number of elements in a type of data
 *	Can also be calculated using the size of a data type and knowing the type of individual data components
 *	Mat3 returns 3 and Mat4 returns 4 because its considered as 3 float3 vectors and 4 float4 vectors
 */
static uint32_t DataTypeCalculateNumberElements(DataType type)
{
	switch (type)
	{
	case DataType::Float:
		return 1;
	case DataType::Float2:
		return 2;
	case DataType::Float3:
		return 3;
	case DataType::Float4:
		return 4;
	case DataType::Mat3:
		return 3;
	case DataType::Mat4:
		return 4;
	case DataType::Int:
		return 1;
	case DataType::Int2:
		return 2;
	case DataType::Int3:
		return 3;
	case DataType::Int4:
		return 4;
	case DataType::Bool:
		return 1;
	default:
		COMETA_WARNING("Unknown Data type for counting number elements");
		return 0;
	}
}

/**
*	Translate from Data Type to GLenum to be used in OpenGL
*/
static GLenum DataTypeToGlEnum(DataType type) {
	switch (type)
	{
	case DataType::Float:
		return GL_FLOAT;
	case DataType::Float2:
		return GL_FLOAT_VEC2;
	case DataType::Float3:
		return GL_FLOAT_VEC3;
	case DataType::Float4:
		return GL_FLOAT_VEC4;
	case DataType::Mat3:
		return GL_FLOAT_MAT3;
	case DataType::Mat4:
		return GL_FLOAT_MAT4;
	case DataType::Int:
		return GL_INT;
	case DataType::Int2:
		return GL_INT_VEC2;
	case DataType::Int3:
		return GL_INT_VEC3;
	case DataType::Int4:
		return GL_INT_VEC4;
	case DataType::Bool:
		return GL_BOOL;
	default:
		COMETA_WARNING("Unknown Data type for counting number elements");
		return 0;
	}
}

#endif // !COMETA_DATATYPE_H