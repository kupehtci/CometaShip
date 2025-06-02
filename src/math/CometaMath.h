#pragma once

#ifndef COMETA_MATH_H
#define COMETA_MATH_H

/**
 * Abstract class that define some static methods for helping mathematical implementations
 */
class CometaMath {

public:

	// Force the class to be abstract
	virtual ~CometaMath() = default;

	static int BoundMax(int value, int max) {
		if (value > max) {
			return max; 
		}
		return value; 
	}

	static float BoundMax(float value, float max) {
		if (value > max) {
			return max; 
		}
		return value; 
	}

	static int BoundMin(int value, int min)
	{
		if (value < min)
		{
			return min;
		}
		return value;
	}

	static float BoundMin(float value, float min)
	{
		if (value < min)
		{
			return min;
		}
		return value;
	}

	static float Scope(float value, float min, float max) {
		if (value > max) {
			return max; 
		}
		else if (value < min) {
			return min; 
		}
		return value; 
	}

	static float Scope01(float value)
	{
		return Scope(value, 0.0f, 1.0f);
	}

	/**
	 * Remap a value from an specied range to another
	 * @param value value to be remaped
	 * @param inMin minimum of the current range
	 * @param inMax maximum of the current range
	 * @param outMin minimum of the desired range
	 * @param outMax maximum of the desired range
	 * @return value proporcionally remaped to the new range
	 */
	static float Map(float value, float inMin, float inMax, float outMin, float outMax) {
		return outMin + (value - inMin) * (outMax - outMin) / (inMax - inMin);
	}

	/**
	 * Calculate the angle between two vectors in 2 dimensions
	 * @param x1 x value of vertor 1
	 * @param y1 y value of vector 1
	 * @param x2 x value of vector 2
	 * @param y2 y value of vector 2
	 * @return angle in radians between both vectors
	 */
	static float Angle2D(float x1, float y1, float x2, float y2) {
		return std::atan2(y2 - y1, x2 - x1);
	}

	/**
	 * Calculate the angle between two vectors in 2 dimensions
	 * @param a first vector
	 * @param b second vector
	 * @return angle in radians between both vectors
	 */
	static float Angle2D(glm::vec3 a, glm::vec3 b)
	{
		return std::atan2(b.y - a.y, b.x - a.x);
	}

	/**
	 * Check if a float value is approximately close to other.
	 * epsilon determine the threshold of close the values must be
	 * @param a	value 1 to check if its close to b
	 * @param b value 2 to check if its close to a
	 * @param epsilon threshold of comparison
	 * @return true is the difference between them is less than epsilon
	 */
	static bool Approximately(float a, float b, float epsilon = 0.001f) {
		return std::abs(a - b) < epsilon;
	}

};

#endif