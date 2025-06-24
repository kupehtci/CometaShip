#include "ZeldaWanderScript.h"
#include "world/Components.h"
#include <gtc/constants.hpp>
#include <gtc/random.hpp>
#include <gtx/vector_angle.hpp>
#include <chrono>
#include <iostream>

ZeldaWanderScript::ZeldaWanderScript(Entity* owner, float maxDistance)
    :  _maxDistance(maxDistance), _speed(2.0f), _waitTimer(0.0f), _changeDirTimer(0.0f),
      _rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count())),
      _angleDist(0.0f, glm::two_pi<float>())
{
    // Store center position 
    _center = owner->GetComponent<Transform>()->position; 


    // Create the initial direction and target
    auto dir2 = glm::circularRand(1.0f);
    _direction = glm::vec3(dir2.x, 0.0f, dir2.y);
    _target = _center + _direction * (_maxDistance * 0.7f); 
}

void ZeldaWanderScript::OnUpdate(float dt)
{
    Transform* transform = _entity->GetComponent<Transform>();
    glm::vec3 toTarget = _target - transform->position;
    float dist = glm::length(toTarget);
    float threshold = 0.2f;

    if (_waitTimer > 0.0f) {
        _waitTimer -= dt;
        if (_waitTimer <= 0.0f) {
            float angle = _angleDist(_rng);
            float radius = _maxDistance * 0.7f * (0.5f + 0.5f * ((float)_rng() / _rng.max()));
            _direction = glm::vec3(cos(angle), 0.0f, sin(angle));
            _target = _center + _direction * radius;
        } else {
            return;
        }
    }

    if (dist > threshold) {
        glm::vec3 moveDir = glm::normalize(toTarget);
        transform->position += moveDir * _speed * dt;

        float desiredYaw = glm::degrees(atan2(moveDir.x, moveDir.z));
        float currentYaw = transform->rotation.y;
        float lerpFactor = 0.05f; 
        float newYaw = currentYaw + (desiredYaw - currentYaw) * lerpFactor;
        transform->rotation.y = newYaw;
    } else {
        _waitTimer = 2.0f;
    }
}

void ZeldaWanderScript::OnInit() {
    std::cout << "ZeldaWanderScript initialized for entity: " << _entity->GetName() << std::endl;
}

void ZeldaWanderScript::OnClose() {
    
}

void ZeldaWanderScript::OnCollisionEnter(Entity* other, Collision* collision) {

}

void ZeldaWanderScript::OnCollisionExit(Entity* other, Collision* collision) {

}