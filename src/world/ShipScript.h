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
    
    // --- For red flash effect ---
    bool _isRed = false;
    float _redTimer = 0.0f;
    
    // --- For smooth tilting ---
    float _targetTilt = 0.0f;
    float _currentTilt = 0.0f;
    float _tiltAmount = 20.0f; // degrees
    float _tiltLerpSpeed = 8.0f;

    // --- Gamepad support ---
    CometaJoystick _gamepadJoystick = CometaJoystick::JOYSTICK_1;
    float _deadZone = 0.15f; 
    
public:
    ShipScript() = default;
    ~ShipScript() override = default;
    
    void OnInit() override {
        COMETA_MSG("[SHIP SCRIPT] Initialized");
    }
    
    void OnUpdate(float deltaTime) override {
        if (!_isAlive || !_entity) return;
        
        // Handle red flash timer
        if (_isRed) {
            _redTimer -= deltaTime;
            if (_redTimer <= 0.0f) {
                // Restore color
                MeshRenderable* renderable = _entity->GetComponent<MeshRenderable>();
                if (renderable && renderable->GetMaterial()) {
                    renderable->GetMaterial()->SetAmbient(glm::vec3(0.1f, 0.1f, 0.8f));
                    renderable->GetMaterial()->SetDiffuse(glm::vec3(0.2f, 0.2f, 0.9f));
                }
                _isRed = false;
            }
        }
        
        Transform* transform = _entity->GetComponent<Transform>();
        if (!transform) return;
        
        float moveX = 0.0f;
        // Keyboard movement
        if (Input::IsKeyPressed(GLFW_KEY_A) || Input::IsKeyPressed(GLFW_KEY_LEFT)) {
            moveX -= 1.0f;
        }
        if (Input::IsKeyPressed(GLFW_KEY_D) || Input::IsKeyPressed(GLFW_KEY_RIGHT)) {
            moveX += 1.0f;
        }
        // Gamepad left joystick movement (CometaJoystick 0, axis 0 = left stick X)
        if (Input::IsJoystickConnected(JOYSTICK_1) && Input::IsJoystickAGamepad(JOYSTICK_1)) {
            auto gamepadInfo = Input::GetGamepadInfo(CometaJoystick::JOYSTICK_1);
            float axisX = gamepadInfo.axes[0];
            if (fabs(axisX) > _deadZone) 
                moveX += axisX;
        }

        
        // --- Tilting logic ---
        if (moveX < 0.0f) {
            _targetTilt = _tiltAmount;
        } else if (moveX > 0.0f) {
            _targetTilt = -_tiltAmount;
        } else {
            _targetTilt = 0.0f;
        }
        // Smoothly interpolate current tilt towards target tilt
        _currentTilt += (_targetTilt - _currentTilt) * std::min(_tiltLerpSpeed * deltaTime, 1.0f);
        
        transform->rotation.z = _currentTilt;
        
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
                COMETA_MSG(std::string("[SHIP SCRIPT] Lives left: ") + std::to_string(_lives));
            }   
            if (_lives == 0) {
                _isAlive = false;
                COMETA_MSG("[SHIP SCRIPT] Ship destroyed!");
            } else {
                COMETA_MSG("[SHIP SCRIPT] Ship hit, but still alive.");
            }
            
            // Visual feedback by turning the ship red for 1 second
            MeshRenderable* renderable = _entity->GetComponent<MeshRenderable>();
            if (renderable && renderable->GetMaterial()) {
                renderable->GetMaterial()->SetAmbient(glm::vec3(0.8f, 0.1f, 0.1f));
                renderable->GetMaterial()->SetDiffuse(glm::vec3(0.9f, 0.2f, 0.2f));

                COMETA_MSG("[SHIP SCRIPT] Ship hit, changing color to red for feedback.");
                _isRed = true;
                _redTimer = 1.0f;
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
    void ResetLives() { _lives = 3; _isAlive = true; }
};

#endif //COMETA_SHIP_SCRIPT_H