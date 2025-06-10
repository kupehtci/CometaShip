//
// Created by Daniel Laplana Gimeno on 6/5/25.
//
#include "physics/PhysicsManager.h"

#include "world/WorldManager.h"
#include "world/ComponentStorage.h"
#include "world/Components.h"
#include "world/Entity.h"
#include "world/ScriptSystem.h"

#include "physics/Collider.h"


#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE

#include "physics/Collision.h"

void PhysicsManager::Init(){

}

void PhysicsManager::Update(){

    if (!_onSimulation) return;


     float dt = Time::GetDeltaTime();

    WorldManager* worldManager = WorldManagerRef;
    std::shared_ptr<World> currentWorld = worldManager->GetCurrentWorld();

    if (currentWorld == nullptr) {
        COMETA_WARNING("[PHYSICS_MANAGER] Current world is not set, cannot calculate physics interactions");
        return;
    }

    ComponentStorage<RigidBody>& rigidBodies = currentWorld->GetComponentRegistry().GetStorage<RigidBody>();

    // STEP PHASE
    for (auto& rb : rigidBodies) {
        if (!rb._enabled) continue;

        ColliderComponent* colliderComp = rb.GetOwner()->GetComponent<ColliderComponent>();
        if (colliderComp == nullptr) {
            COMETA_MSG("[PHYSICS_MANAGER] Rigidbody owner with UID ", rb.GetOwner()->GetUID() , " has no collider");
            continue;
        }

        Transform* rbTransform = rb.GetOwner()->GetComponent<Transform>();
        rb.Init();

        // Linear Step
        // rb._force += rb._mass * _gravity;
        rb._force += rb.IsAffectedByGravity() ? (rb._mass * _gravity) : glm::vec3(0.0f, 0.0f, 0.0f);

        rb._linearVelocity += rb._force / rb._mass * dt;
        rbTransform->position += rb._linearVelocity * dt;

        rb._force = glm::vec3(0.0f);

        // Angular Step
        rb._angularVelocity += rb._inverseInertiaTensor * rb._torque * dt;
        glm::quat rotation = glm::quat(glm::vec3(rb._angularVelocity * dt));
        rbTransform->rotation = glm::degrees(glm::eulerAngles(rotation * glm::quat(glm::radians(rbTransform->rotation))));

        // rb._angularVelocity = glm::vec3(0.0f);
        rb._torque = glm::vec3(0.0f);
    }


    // BROAD PHASE with improved collision handling
    std::vector<Collision> collisions;
    ComponentStorage<ColliderComponent>& colliders = currentWorld->GetComponentRegistry().GetStorage<ColliderComponent>();

    for (size_t i = 0; i < colliders.Size(); ++i) {
        for (size_t j = i + 1; j < colliders.Size(); ++j) {
            ColliderComponent* colA = &colliders[i];
            ColliderComponent* colB = &colliders[j];

            if (colA == colB) continue;
            if (colA->GetCollider() == nullptr || colB->GetCollider() == nullptr) continue;

            Transform* transformA = colA->GetOwner()->GetComponent<Transform>();
            Transform* transformB = colB->GetOwner()->GetComponent<Transform>();

            CollisionPoint point = CollisionDispatcher::Dispatch(colA->GetCollider(), transformA, colB->GetCollider(), transformB);

            // If collided, store the collision to be processed in the next step
            if (point.collided)
            {
                Collision collision = {colA, colB, point};
                collisions.emplace_back(colA, colB, point);
                
                // Notify ScriptSystem about the collision
                ScriptManagerRef->ProcessCollision(colA->GetOwner(), colB->GetOwner(), &collision, true);
            }
            else
            {
                Collision collision = {colA, colB, point};
                ScriptManagerRef->ProcessCollision(colA->GetOwner(), colB->GetOwner(), &collision, false);
            }
        }
    }


    // COLLISION RESOLUTION with Baumgarte Stabilization

    const float slop = 0.01f;   // Penetration slop

    for (auto& col : collisions) {
        RigidBody* rbA = col.colliderA->GetOwner()->GetComponent<RigidBody>();
        RigidBody* rbB = col.colliderB->GetOwner()->GetComponent<RigidBody>();

        // COMETA_MSG("[PHYSICS MANAGER] Check collision between ", col.colliderA->GetOwner()->GetUID(), " and ", col.colliderB->GetOwner()->GetUID(), " [PHYSICS MANAGER]");

        Transform* transformA = col.colliderA->GetOwner()->GetComponent<Transform>();
        Transform* transformB = col.colliderB->GetOwner()->GetComponent<Transform>();

        // Skip if both objects are static
        // Objects are static if Mass is 0
        float totalMass = (rbA ? rbA->_mass : 0.0f) + (rbB ? rbB->_mass : 0.0f);
        if (totalMass <= 0.01f) continue;


        // Calculate relative velocity
        // glm::vec3 relativeVel = (rbB ? rbB->_linearVelocity : glm::vec3(0.0f)) -
           //                    (rbA ? rbA->_linearVelocity : glm::vec3(0.0f));

        // Calculate point of application relative to center of mass
        glm::vec3 rA = col.point.point - transformA->position;
        glm::vec3 rB = col.point.point - transformB->position;

        // Calculate angular velocities at point of contact
        glm::vec3 angularVelA = rbA ? glm::cross(rbA->_angularVelocity, rA) : glm::vec3(0.0f);
        glm::vec3 angularVelB = rbB ? glm::cross(rbB->_angularVelocity, rB) : glm::vec3(0.0f);

        // Total relative velocity including both linear and angular
        glm::vec3 relativeVel = (rbB ? rbB->_linearVelocity + angularVelB : glm::vec3(0.0f)) -
                               (rbA ? rbA->_linearVelocity + angularVelA : glm::vec3(0.0f));

        // Calculate relative velocity along normal
        float velAlongNormal = glm::dot(relativeVel, col.point.normal);

        // Don't resolve if objects are separating    if (velAlongNormal > 0.0f) continue;
        float restitution = 0.1f; // Coefficient of restitution

        // Baumgarte stabilization term calculation
        float penetration = std::max(col.point.length - slop, 0.0f);
        float baumgarteTerm = (_beta / dt) * penetration;

        // Impulse scalar
        float j = -(1.0f + restitution) * velAlongNormal;
        j += baumgarteTerm; // Add Baumgarte term
        j /= totalMass;

        // Apply impulse
        glm::vec3 impulse = j * col.point.normal;


        // ANGULAR IMPULSE



        // Calculate angular impulse
        if (rbA && rbA->_mass > 0.0f) {
            glm::vec3 angularImpulse = glm::cross(rA, impulse);
            rbA->_angularVelocity -= rbA->_inverseInertiaTensor * angularImpulse * (rbA->_mass / totalMass);
            // rbA->_torque += angularImpulse;
        }

        if (rbB && rbB->_mass > 0.0f) {
            glm::vec3 angularImpulse = glm::cross(rB, impulse);
            rbB->_angularVelocity += rbB->_inverseInertiaTensor * angularImpulse * (rbB->_mass / totalMass);
            // rbB->_torque += angularImpulse;
        }

        // END OF ANGULAR IMPULSE

        if (rbA && rbA->_mass > 0.0f) {
            float massRatioA = rbA->_mass / totalMass;
            rbA->_linearVelocity -= impulse * massRatioA;

            transformA->position -= col.point.normal * penetration * _beta * massRatioA;
        }

        if (rbB && rbB->_mass > 0.0f) {
            float massRatioB = rbB->_mass / totalMass;
            rbB->_linearVelocity += impulse * massRatioB;

            transformB->position += col.point.normal * penetration * _beta * massRatioB;
        }

        // Apply friction
        const float frictionCoeff = 0.3f;

        // Calculate tangent vector
        glm::vec3 tangent = relativeVel - (col.point.normal * velAlongNormal);
        float tangentLength = glm::length(tangent);

        if (tangentLength > 0.0001f) {
            tangent = glm::normalize(tangent);
            float jt = -glm::dot(relativeVel, tangent);
            jt /= totalMass;

            // Coulomb's law
            float maxFriction = j * frictionCoeff;
            jt = glm::clamp(jt, -maxFriction, maxFriction);

            glm::vec3 frictionImpulse = jt * tangent;

            if (rbA && rbA->_mass > 0.0f) {
                rbA->_linearVelocity -= frictionImpulse * (rbA->_mass / totalMass);
            }

            if (rbB && rbB->_mass > 0.0f) {
                rbB->_linearVelocity += frictionImpulse * (rbB->_mass / totalMass);
            }
        }
    }
}

void PhysicsManager::Close(){

}
