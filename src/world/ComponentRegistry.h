#ifndef COMETA_COMPONENT_REGISTRY_H
#define COMETA_COMPONENT_REGISTRY_H

// #include "world/Entity.h"
#include "world/Components.h"
#include "world/ComponentStorage.h"

/**
 * Registry for the Component Storages of each type
 * On addition of new components, they should be added into the tuple in order to be stored within the registries
 */
class ComponentRegistry {

private:
	std::tuple<
		ComponentStorage<Transform>,
		ComponentStorage<MeshRenderable>,
		ComponentStorage<SpriteRenderable>,
		ComponentStorage<PointLight>,
		ComponentStorage<DirectionalLight>,
		ComponentStorage<ColliderComponent>,
		ComponentStorage<RigidBody>,
		ComponentStorage<Script>,
		ComponentStorage<Tag>
		> _components;

public: 
	ComponentRegistry() = default;

	/**
	 * Add a pointer to a component as the new component for that entity
	 * @tparam T Type of component to add
	 * @param uid Unique ID of the Entity to add the component to
	 * @param component Component to Add to that entity
	 */
	template<typename T>
	void AddComponent(const uint32_t& uid, const T* component) {
		GetStorage<T>().Add(uid, component);
	}

	/**
	 * Create the specified T component and assign to the Entity
	 * @tparam T new component to be created for the entity
	 * @param uid Unique ID of the Entity to create component for
	 * @param ent Entity to assign the component to.
	 * @return pointer to the new component created
	 */
	template<typename T>
	T* CreateComponent(const uint32_t& uid){
		return GetStorage<T>().Create(uid);
	}

	/**
	 * Remove the component from the entity
	 * @tparam T Component to delete
	 * @param uid Unique ID of the entity to remove component of
	 * @param ent Entity pointer to delete the component from
	 */
	template<typename T>
	void RemoveComponent(const uint32_t& uid) {
		GetStorage<T>().Pop(uid);
	}

	/**
	 * Get a pointer to the specified component that belongs to that Entity
	 * @tparam T Component to get
	 * @param ent Entity who belongs the component to
	 * @return Pointer to the component stored in the ComponentStorage
	 */
	template<typename T>
	T* GetComponent(const uint32_t& uid) {
		// Get returns a pointer to the element stored in the Sparse Set
		return GetStorage<T>().Get(uid);
	}

	template<typename T>
	bool HasComponent(const uint32_t& uid) {
		return GetStorage<T>().Contains(uid);
	}

	/**
	* Retrieve the storage for an specific type of component
	*/
	template<typename T>
	ComponentStorage<T>& GetStorage() {
		return std::get<ComponentStorage<T>>(_components);
	}

	// inline void Debug()
	// {
	// 	std::cout << "=== ComponentRegistry ===" << std::endl;
	// 	for (auto storage : _components)
	// 	{
	// 		std::cout << "StorageComponent type: " << typeid(storage).name() << "\n";
	// 		for (auto it = storage.begin(); it != storage.end(); ++it)
	// 		{
	// 			std::cout << it->first << ": " << it->second << "\n";
	// 		}
	// 	}
	// }
};

#endif // COMETA_COMPONENT_REGISTRY_H