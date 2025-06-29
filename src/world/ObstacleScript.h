#ifndef COMETA_OBSTACLE_SCRIPT_H
#define COMETA_OBSTACLE_SCRIPT_H

#include <iostream>
#include "world/BaseScript.h"
#include "world/Entity.h"
#include "world/Components.h"
#include "world/WorldManager.h"
#include "debug/Assertion.h"

class ObstacleScript final : public BaseScript {
private:
    float _speed = 5.0f;           // Units per second
    float _destroyYPosition = -20.0f; // Z position at which to deactivate the obstacle

    // Time in seconds before the obstacle is destroyed
    float _lifeSpan = 30.0f; 
    // Maximum lifespan for reset purposes
    float _maxLifeSpan = 30.0f; 
    bool _expired = false;
    
public:
    ObstacleScript() = default;
    
    // Constructor with speed parameter
    explicit ObstacleScript(float speed) : _speed(speed) {}
    
    ~ObstacleScript() override = default;
    
    void OnInit() override {
        COMETA_MSG("[OBSTACLE SCRIPT] Initialized");
        
        if (_entity) {
            RigidBody* rb = _entity->GetComponent<RigidBody>();
            if (rb) {
                rb->SetLinearVelocity(glm::vec3(0.0f, _speed, 0.0f));
            }
        }
    }
    
    void OnUpdate(float deltaTime) override {
        if (!_entity) return;
        Transform* transform = _entity->GetComponent<Transform>();
        if (!transform) return;
        if (transform->position.y < _destroyYPosition) {
            _expired = true;
            return;
        }
        RigidBody* rb = _entity->GetComponent<RigidBody>();
        if (rb) {
            glm::vec3 currentVel = rb->GetLinearVelocity();
            if (currentVel.y != -_speed) {
                rb->SetLinearVelocity(glm::vec3(currentVel.x, -_speed, currentVel.z));
            }
        }
        _lifeSpan -= deltaTime;
        if (_lifeSpan <= 0.0f) {
            _expired = true;
        }
    }
    
    void OnClose() override {
        COMETA_MSG("[OBSTACLE SCRIPT] Closed");
    }
    
    void OnCollisionEnter(Entity* other, Collision* collision) override {
        COMETA_MSG("[OBSTACLE SCRIPT] Collision Enter");
        
        // Check if entity and other entity are valid
        if (!_entity || !other) return;
        
        // Check if we collided with the player
        Tag* otherTag = other->GetComponent<Tag>();
        if (otherTag && otherTag->GetTag() == "player") {
            COMETA_MSG("[OBSTACLE SCRIPT] Hit player!");
            
            // Visual feedback - make the obstacle brighter
            MeshRenderable* renderable = _entity->GetComponent<MeshRenderable>();
            if (renderable && renderable->GetMaterial()) {
                renderable->GetMaterial()->SetAmbient(glm::vec3(0.0f, 0.8f, 0.0f));
                renderable->GetMaterial()->SetDiffuse(glm::vec3(1.0f, 0.7f, 0.0f));
                renderable->GetMaterial()->SetSpecular(glm::vec3(1.0f, 0.9f, 0.5f));
            }
        }
    }
    
    void OnCollisionExit(Entity* other, Collision* collision) override {
        COMETA_MSG("[OBSTACLE SCRIPT] Collision Exit");
    }
    
    // ------------ GETTERS AND SETTERS ---------
    float GetSpeed() const { return _speed; }
    void SetSpeed(float speed) { _speed = speed; }
    bool IsExpired() const { return _expired; }
    
    void Reset(float speed) { 
        _speed = speed; 
        _lifeSpan = _maxLifeSpan; 
        _expired = false; 
    }

    void Reset() {
        _speed = 5.0f;
        _lifeSpan = _maxLifeSpan;
        _expired = false;
    }
};

#endif //COMETA_OBSTACLE_SCRIPT_H