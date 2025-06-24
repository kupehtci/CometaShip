#ifndef COMETA_ENTITY_H
#define COMETA_ENTITY_H

#include <iostream>
#include <string>

#include "world/ComponentRegistry.h"
#include "world/World.h"
class World;


// Be careful with World circular dependency by ComponentRegistry class

/**
 * Entity class container.
 * Acts as a container with no functionality, for allowing components to get attached to its UID.
 * It offers interface methods to work directly with its components
 */
class Entity {

private:
    // unique identifier of the entity
    uint32_t _uid = 0;

    // Used for assigning new unique IDs to each new entity
    static uint32_t AvailableUid;

    // Name of the entity
    std::string _name = "Default";

    /**
     * World were the entity belongs to
     */
    World* _parentWorld = nullptr;

public:
    /**
     * Default constructor for Entity class.
     * As no name is defined, Cometa_{uid} is used as the name
     */
    Entity();
    ~Entity();

    /**
     * Name assigned constructor
     * @param name {string} name for the entity.
     */
    explicit Entity(const std::string& name);

    // ------------ COMPONENTS MANAGEMENT ------------

    /**
     * Create a component and attach it to the entity by inserting it in the corresponding @class ComponentStorage
     * using the entity ID as a reference
     * @tparam T Type of component to create
     * @return {T*} Pointer to the newly created component
     */
    template<typename T>
    T* CreateComponent()
    {
        T* newComponent = _parentWorld->_componentRegistry.CreateComponent<T>(this->GetUID());
        newComponent->SetOwner(this);
        return newComponent;
    }

    /**
     * Retrieve a component from the class.
     * Usage:
     * @code
     *  // Use it like
     *  auto* transform = entity->GetComponent<Transform>();
     * @endcode
     * @tparam T type of the component to retrieve
     * @return {T*} Component pointer
     */
    template<typename T>
    T* GetComponent()
    {
        return _parentWorld->_componentRegistry.GetComponent<T>(this->_uid);
    }

    template<typename T>
    void RemoveComponent()
    {
        _parentWorld->_componentRegistry.RemoveComponent<T>(this->_uid);
    }


    template<typename T>
    bool HasComponent()
    {
        return _parentWorld->_componentRegistry.HasComponent<T>(this->_uid);
    }


public:
    // ------------ OPERATOR OVERLOAD ------------
    friend std::ostream& operator<<(std::ostream& os, const Entity& entity)
    {
        os << "Entity UID: " << entity._uid << std::endl;
        return os;
    }

    bool operator==(const Entity& other) const {
        return _uid == other._uid;
    }

    bool operator!=(const Entity& other) const {
        return _uid != other._uid;
    }

public:
    // --------- GETTERS AND SETTERS ---------
    [[nodiscard]] uint32_t GetUID() const { return _uid;}
    [[nodiscard]] std::string GetName() const { return _name;}
    [[nodiscard]] World* GetParentWorld() const { return _parentWorld;}

    void SetName(const std::string& name) { _name = name;}

    friend class World;
};



#endif //COMETA_ENTITY_H