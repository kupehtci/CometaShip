#ifndef COMETA_ENTITY_H
#define COMETA_ENTITY_H

#include <iostream>
#include <string>

#include "world/ComponentRegistry.h"
#include "world/World.h"
class World;


// Be careful with World circular dependency by ComponentRegistry class
class Entity {

private:
    uint32_t _uid = 0;
    static uint32_t AvailableUid;

    std::string _name = "Default";

    World* _parentWorld = nullptr;

public:
    Entity();
    ~Entity();
    explicit Entity(const std::string& name);

    // ------------ COMPONENTS MANAGEMENT ------------
    template<typename T>
    T* CreateComponent()
    {
        T* newComponent = _parentWorld->_componentRegistry.CreateComponent<T>(this->GetUID());
        newComponent->SetOwner(this);
        // newComponent->Init();
        return newComponent;
    }

    template<typename T>
    T* GetComponent()
    {
        return _parentWorld->_componentRegistry.GetComponent<T>(this->_uid);
    }

    template<typename T>
    void RemoveComponent()
    {
        // std::cout << "Removing component of type: " << typeid(T).name() << std::endl;
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