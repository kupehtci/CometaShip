#ifndef COMETA_SHIP_SCRIPT_H
#define COMETA_SHIP_SCRIPT_H

#include <iostream>
#include "world/BaseScript.h"
#include "world/Entity.h"
#include "world/Components.h"
#include "input/Input.h"
#include "debug/Assertion.h"

class ShipScript final : public BaseScript {
private:
    float _moveSpeed = 3.0f;      
    float _moveBounds = 5.0f;       
    bool _isAlive = true;
    unsigned int _lives = 3;
    
public:
    ShipScript() = default;
    ~ShipScript() override = default;
    
    void OnInit() override {
        COMETA_MSG("[SHIP SCRIPT] Initialized");
    }
    
    void OnUpdate(float deltaTime) override {
        if (!_isAlive || !_entity) return;
        
        Transform* transform = _entity->GetComponent<Transform>();
        if (!transform) return;
        
        float moveX = 0.0f;
        
        if (Input::IsKeyPressed(GLFW_KEY_A) || Input::IsKeyPressed(GLFW_KEY_LEFT)) {
            COMETA_MSG("[SHIP SCRIPT] Moving left");
            moveX -= 1.0f;
        }
        
        if (Input::IsKeyPressed(GLFW_KEY_D) || Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
            COMETA_MSG("[SHIP SCRIPT] Moving right");
            moveX += 1.0f;
        }
        
        if (moveX != 0.0f) {
            float newX = transform->position.x + moveX * _moveSpeed * deltaTime;
            
            // Clamp position within bounds
            newX = glm::clamp(newX, -_moveBounds, _moveBounds);
            
            // Update position
            transform->position.x = newX;
        }
    }
    
    void OnClose() override {
        COMETA_MSG("[SHIP SCRIPT] Closed");
    }
    
    void OnCollisionEnter(Entity* other, Collision* collision) override {
        COMETA_MSG("[SHIP SCRIPT] Collision Enter");
        
        // Check if we collided with an obstacle
        Tag* otherTag = other->GetComponent<Tag>();
        if (otherTag && otherTag->GetTag() == "obstacle") {
            COMETA_MSG("[SHIP SCRIPT] Hit by obstacle!");
            
            // Decrease lives
            if (_lives > 0) {
                _lives--;
                COMETA_MSG("[SHIP SCRIPT] Lives left: " << _lives);
            }   
            if (_lives == 0) {
                _isAlive = false;
                COMETA_MSG("[SHIP SCRIPT] Ship destroyed!");
            } else {
                COMETA_MSG("[SHIP SCRIPT] Ship hit, but still alive.");
            }
            
            // Visual feedback - turn the ship red
            MeshRenderable* renderable = _entity->GetComponent<MeshRenderable>();
            if (renderable && renderable->GetMaterial()) {
                renderable->GetMaterial()->SetAmbient(glm::vec3(0.8f, 0.1f, 0.1f));
                renderable->GetMaterial()->SetDiffuse(glm::vec3(0.9f, 0.2f, 0.2f));
            }
        }
    }
    
    void OnCollisionExit(Entity* other, Collision* collision) override {
        COMETA_MSG("[SHIP SCRIPT] Collision Exit");
    }
    
    // Getters/Setters
    bool IsAlive() const { return _isAlive; }
    void SetAlive(bool alive) { _isAlive = alive; }
    
    Entity* GetOwner() const { return _entity; }
    void SetOwner(Entity* entity) { _entity = entity; }

    unsigned int GetLives() const { return _lives; }
    void SetLives(unsigned int lives) { _lives = lives; }
};

#endif //COMETA_SHIP_SCRIPT_H