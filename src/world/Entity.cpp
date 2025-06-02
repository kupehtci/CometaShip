#include "Entity.h"

uint32_t Entity::AvailableUid = 1;

Entity::Entity() {
    _uid = AvailableUid;
    AvailableUid++;
    _name = "Cometa_" + std::to_string(_uid);
}

Entity::Entity(const std::string& name) {
    _uid = AvailableUid;
    AvailableUid++;
    _name = name;
}

Entity::~Entity()
{

}

