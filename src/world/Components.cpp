#include "world/Components.h"

#include "world/Entity.h"

void RigidBody::Init()
{

    // Get inertia tensor and inverse one from the Collider Component
    const auto* coll = GetOwner()->GetComponent<ColliderComponent>();
    if (coll == nullptr){
        COMETA_ERROR("[RIGIDBODY] Collider must be set to entity before the Collider creation");
        return;
    }

    _inertiaTensor = coll->GetCollider()->CalculateInertiaTensor(_mass);
    _inverseInertiaTensor = coll->GetCollider()->CalculateInverseInertiaTensor(_mass);
}

void RigidBody::Reset()
{
    // Linear
    _linearVelocity = { 0.0f, 0.0f, 0.0f };
    _force = { 0.0f, 0.0f, 0.0f };
    _mass = 1.0f;

    _enabled = true;

    // Angular
    _torque = { 0.0f, 0.0f, 0.0f };
    _angularVelocity = { 0.0f, 0.0f, 0.0f };
    _inertiaTensor = glm::mat3();
    _inverseInertiaTensor = glm::mat3();

    Init();
}
