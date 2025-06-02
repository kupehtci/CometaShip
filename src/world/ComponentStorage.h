#ifndef COMETA_COMPONENT_STORAGE_H
#define COMETA_COMPONENT_STORAGE_H

#include "types/SparseSet.h"

/**
 * Component storage is an Sparse Set that stores the object more efficiently than ordered maps <id, Entity>
 * @tparam T typename of the component
 */
template<typename T>
class ComponentStorage : public SparseSet<T>{

public:
	ComponentStorage()
	{
		// std::cout << "ComponentStorage() for component: " << typeid(T).name() << std::endl;
	};

	/**
	 * Create the component with default constructor and return a pointer to it
	 * Similar to add but component is created by this function and not passed to it.
	 * This avoids unnecessary copies of the component and lose of reference
	 * @param index Index to insert the component to. This need to be the Entity UID.
	 * @returns Pointer to the new created component
	 */
	T* Create(size_t index)
	{
		if (this->Contains(index))
		{
			std::cout << "ComponentStorage() for component: " << typeid(T).name() << " already has the component" << std::endl;
			return this->Get(index);
		}

		// Increase dense and sparse capacity
		if (this->_size >= this->_denseCapacity) {
			this->_denseCapacity = this->_capacity = this->_denseCapacity * 2;
			this->_dense.resize(this->_denseCapacity);
			this->_sparse.resize(this->_capacity, -1);
		}

		// std::cout << "Created component for Entity: " << index << " new dense capacity: " << this->_denseCapacity << std::endl;

		// Create the new component in the Dense
		this->_sparse[index] = this->_size;
		this->_dense.push_back(T());
		this->_denseIndex.push_back(index);

		this->_size++;
		return &this->_dense[this->_size - 1];
	}
};

#endif