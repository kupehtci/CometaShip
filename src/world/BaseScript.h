#ifndef COMETA_BASE_SCRIPT_H
#define COMETA_BASE_SCRIPT_H

// Don't include collision or entity classes as they create circular dependency,
// implement the functions over the .cpp file
class Collision;
class Entity;

class BaseScript {
public:
    Entity* _entity = nullptr;

public:
    virtual ~BaseScript() = default;

    virtual void OnInit() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnClose() = 0;

    virtual void OnCollisionEnter(Entity* other, Collision* collision) = 0;
    virtual void OnCollisionExit(Entity* other, Collision* collision) = 0;
};

#endif //COMETA_BASE_SCRIPT_H
