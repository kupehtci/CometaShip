#ifndef COMETA_SPARSE_SET_H
#define COMETA_SPARSE_SET_H

#include <iostream>
#include <vector>
#include "debug/Assertion.h"

template<typename T>
class SparseSet {

protected: 
	std::vector<T> _dense;
	std::vector<int> _denseIndex;
	std::vector<int> _sparse;

	size_t _size = 0;					// Number of elements within the Sparse Set. Its also a pointer to the end of the sparse set
	size_t _capacity = 100;				// Maximum capacity of the Sparse.
	size_t _denseCapacity = 100;		// Maximum capacity of the Dense.

	size_t _lastInsertedSparse = 0;		// Keeps track of the last item inserted in the sparse. This is helpful for popping items (removing efficiently)


public: 
	SparseSet() {
		_size = 0;
		_capacity = 100;
		_denseCapacity = 100;

		_dense.reserve(_denseCapacity);
		//_dense = std::vector<T>(_denseCapacity);
		_denseIndex.reserve(_denseCapacity);  // = std::vector<int>(_denseCapacity);
		_sparse = std::vector<int>(_capacity, -1);		// initialize with all values to -1. Used for checking an empty value
	}

	void Add(size_t index, const T& value) {

		if (Contains(index))
		{
			COMETA_WARNING("SparseSet: Tried to insert a currently existing item");
			return;
		}

		// Increase dense and dense Index capacity if full
		if (_size >= _denseCapacity) {
			_denseCapacity = _denseCapacity * 2;
			_dense.reserve(_denseCapacity);
			_denseIndex.reserve(_denseCapacity);
		}
		// Increase sparse if index to insert is out of scope
		while (_capacity <= index)
		{
			_capacity *= 2;
			_sparse.resize(_capacity, -1);
		}

		std::cout << "Added value at index: " << index << " dense capacity: " << _denseCapacity << std::endl;

		_sparse[index] = _size;
		_dense.push_back(value);
		_denseIndex.push_back(static_cast<int>(index));

		_size++;
	}

	void Pop(size_t index) {
		if (!Contains(index)) return;

		// Swap deleted item with the last one and update both sparse's values
		int lastDenseIndex = _size-1;

		_dense[_sparse[index]] = _dense[lastDenseIndex];
		_denseIndex[_sparse[index]] = _denseIndex[lastDenseIndex];
		_sparse[_denseIndex[lastDenseIndex]] = _sparse[index];

		_sparse[index] = -1;
		_size--;
	}

	T* Get(const size_t index) {
		if (!Contains(index)) {
			return nullptr;
		}

		return &_dense[_sparse[index]];
	}

	T* GetFirst()
	{
		return _size == 0 ? nullptr : &_dense[0];
	}

	T* GetLast() {
		return _size == 0 ? nullptr : &_dense[_size - 1];
	}
	
	[[nodiscard]] bool Contains (const size_t value) const {
		return (value < _capacity && _sparse[value] != -1);
	}

	/**
	 * Check if dense contains the Data.
	 * The T class must have comparison "==" operator implemented
	 * @param data data to check if its contained in the sparse
	 * @return
	 */
	bool Contains(T data) const {
		for (size_t i = 0; i < _size; i++) {
			if (_dense[i] == data) {
				return true;
			}
		}

		return false;
	}

	void Clear() {
		_size = 0;
	}

	int GetDenseIndex(size_t i)
	{
		return _denseIndex[i];
	}

	void Print() const {
		for (size_t i = 0; i < _size; i++) {
			std::cout << "SparseSet [" << i << "] : " << _dense[i] << " dense capacity:  " << std::endl;
		}
	}

	void PrintIndex() const {
		std::cout << "SparseSet print index: " << std::endl;
		for (size_t i = 0; i < _capacity; i++) {
			if (_sparse[i] >= 0) {
				std::cout << "Sparse[" << i << "] contains dense index: " << _sparse[i] << " that contains: " << _dense[_sparse[i]] << std::endl;
			}
		}
	}


	// --------- OPERATOR OVERLOAD ------------

	// Take into account that operator[] methods are designed to access and travel through the Dense of the sparse set.
	// to get the Dense item that is attached to a certain index contained in the Sparse use `T* Get()` method

	T& operator[](size_t index) {
		if (index >= _size) { COMETA_WARNING("[SPARSE_SET] Accessing not contained dense item"); }
		return _dense[index];
	}

	const T& operator[](size_t index) const {
		if (index >= _size) { COMETA_WARNING("[SPARSE_SET] Accessing not contained dense item"); }
		return _dense[index];
	}

	// --------- GETTERS AND SETTERS ---------
public: 
	[[nodiscard]] size_t Size() const { return _size;  }
	[[nodiscard]] size_t Capacity() const{ return _capacity;  }
	[[nodiscard]] size_t DenseCapacity() const{ return _denseCapacity;  }

	// ITERATION IMPLEMENTATION
	// This allows to use the class with a for each loop, example:
	// for (auto element : example_sparse_set){} ...
	typename std::vector<T>::iterator begin() {return _dense.begin();}
	typename std::vector<T>::iterator end() {return _dense.begin() + _size;}

	typename std::vector<T>::const_iterator begin() const { return _dense.begin(); }
	typename std::vector<T>::const_iterator end() const { return _dense.begin() + _size; }
};

#endif