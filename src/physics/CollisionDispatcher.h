//
// Created by Daniel Laplana Gimeno on 6/5/25.
//

#ifndef COMETA_COLLISION_DISPATCHER_H
#define COMETA_COLLISION_DISPATCHER_H

#include <glm.hpp>

// define the classes
class Transform;
class Collider;

/**
 * Point of contact between 2 colliders
 * Its calculated by the PhysicsManager on the Broad Phase
 */
struct CollisionPoint
{
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 point;
    glm::vec3 normal;
    float length;
    bool collided;
};

using CollisionDispatchFunction = CollisionPoint(*)(const Collider*, const Transform*, const Collider*, const Transform*);

/**
 * Helps in the dispatching of the collision events
 * Uses a dispatch matrix to resolve collisions using different functions
 */
class CollisionDispatcher
{

private:
    static CollisionDispatchFunction collisionDispatcher[2][2];

public:
    static CollisionPoint IntersectBoxSphere(const Collider* collider, const Transform* transform, const Collider* otherCollider, const Transform* otherTransform);
    static CollisionPoint IntersectSphereSphere(const Collider* collider, const Transform* transform, const Collider* otherCollider, const Transform* otherTransform);
    static CollisionPoint IntersectBoxBox(const Collider* collider, const Transform* transform, const Collider* otherCollider, const Transform* otherTransform);

    static CollisionPoint Dispatch(const Collider* collider, const Transform* transform, const Collider* otherCollider, const Transform* otherTransform);
};


#endif //COMETA_COLLISION_DISPATCHER_H
