//
// Created by Daniel Laplana Gimeno on 6/5/25.
//

#include "CollisionDispatcher.h"

#include <complex>

#include "physics/Collider.h"
#include "physics/Collider.h"
#include "world/Components.h"

#include <iostream>

// Enable this if want to debug collisions using the console
// #define PHYSICS_DEBUG

glm::mat3 EulerAnglesXYZ(const glm::vec3& angles) {
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), angles.x, glm::vec3(1, 0, 0));
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), angles.y, glm::vec3(0, 1, 0));
    glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), angles.z, glm::vec3(0, 0, 1));
    return glm::mat3(rotZ * rotY * rotX); // XYZ order
}

// Need to swap the lower ColliderType to the first argument and higher ColliderType to the second argument
CollisionPoint CollisionDispatcher::IntersectBoxSphere(const Collider* collider, const Transform* transform, const Collider* otherCollider, const Transform* otherTransform)
{
#ifdef PHYSICS_DEBUG
    std::cout << "Collision detected Box sphere" << std::endl;
#endif

    CollisionPoint result{};

    // Cast colliders to their specific types
    const auto* box = dynamic_cast<const BoxCollider*>(collider);
    const auto* sphere = dynamic_cast<const SphereCollider*>(otherCollider);

    // Get sphere center in world space
    glm::vec3 sphereCenter = otherTransform->position;
    float sphereRadius = sphere->GetRadius();

    // Get box transform
    glm::vec3 boxCenter = transform->position;
    glm::vec3 boxHalfSize = box->GetExtents();

    // Get box rotation matrix
    glm::mat3 boxRotation = glm::mat3(EulerAnglesXYZ(
        {glm::radians(transform->rotation.x),
         glm::radians(transform->rotation.y),
         glm::radians(transform->rotation.z)}
    ));

    // Transform sphere center to box local space
    glm::vec3 localSphereCenter = glm::transpose(boxRotation) * (sphereCenter - boxCenter);

    // Find closest point on box to sphere center in local space
    glm::vec3 closestPoint;
    closestPoint.x = glm::clamp(localSphereCenter.x, -boxHalfSize.x, boxHalfSize.x);
    closestPoint.y = glm::clamp(localSphereCenter.y, -boxHalfSize.y, boxHalfSize.y);
    closestPoint.z = glm::clamp(localSphereCenter.z, -boxHalfSize.z, boxHalfSize.z);

    // Transform closest point back to world space
    glm::vec3 worldClosestPoint = boxRotation * closestPoint + boxCenter;

    // Calculate distance and normal
    glm::vec3 delta = sphereCenter - worldClosestPoint;
    float distance = glm::length(delta);

    // Check for collision
    if (distance <= sphereRadius) {
        result.collided = true;
        result.point = worldClosestPoint;
        result.normal = glm::normalize(delta);
        result.length = sphereRadius - distance;

        // Calculate contact points
        result.a = worldClosestPoint;
        result.b = sphereCenter - result.normal * sphereRadius;
    }

    return result;
}

CollisionPoint CollisionDispatcher::IntersectSphereSphere(const Collider* collider, const Transform* transform, const Collider* otherCollider, const Transform* otherTransform)
{
#ifdef PHYSICS_DEBUG
    std::cout << "Collision detected sphere sphere" << std::endl;
#endif

    CollisionPoint point{};
    const auto* a = dynamic_cast<const SphereCollider* >(collider);
    const auto* b = dynamic_cast<const SphereCollider* >(otherCollider);

    glm::vec3 centerA = transform->position + a->GetCenter();
    glm::vec3 centerB = otherTransform->position + b->GetCenter();

    glm::vec3 del = centerB - centerA;
    glm::vec3 norm = glm::normalize(del);

    float distance = glm::length(del);
    float radiusSum = a->GetRadius() + b->GetRadius();


    point.a = centerA + (norm * a->GetRadius());
    point.b = centerB - (norm * b->GetRadius());
    point.normal = norm;
    point.length = (radiusSum) - distance;
    point.collided = (distance < radiusSum);

    return point;
}

CollisionPoint CollisionDispatcher::IntersectBoxBox(const Collider* collider, const Transform* transform, const Collider* otherCollider, const Transform* otherTransform)
{
#ifdef PHYSICS_DEBUG
    std::cout << "Collision detected Box Box" << std::endl;
#endif

     CollisionPoint point;

    const auto* box1 = dynamic_cast<const BoxCollider*>(collider);
    const auto* box2 = dynamic_cast<const BoxCollider*>(otherCollider);

    // Get box centers and extents
    glm::vec3 center1 = transform->position;
    glm::vec3 center2 = otherTransform->position;
    glm::vec3 extents1 = box1->GetExtents();
    glm::vec3 extents2 = box2->GetExtents();

    // Get rotation matrices
    glm::mat3 rotation1 = glm::mat3(EulerAnglesXYZ(
        {glm::radians(transform->rotation.x),
        glm::radians(transform->rotation.y),
        glm::radians(transform->rotation.z)}
    ));

    glm::mat3 rotation2 = glm::mat3(EulerAnglesXYZ(
        {glm::radians(otherTransform->rotation.x),
        glm::radians(otherTransform->rotation.y),
        glm::radians(otherTransform->rotation.z)}
    ));

    // Simple distance check first
    float maxDistance = glm::length(extents1) + glm::length(extents2);
    if (glm::distance(center1, center2) > maxDistance) {
        point.collided = false;
        return point;
    }

    // Transform box2 into box1's local space
    glm::vec3 d = center2 - center1;
    glm::mat3 rotationToLocal = glm::transpose(rotation1);
    glm::vec3 localD = rotationToLocal * d;

    // Get axes for SAT test
    std::vector<glm::vec3> axes;
    // Box1 axes
    axes.push_back(rotation1[0]);
    axes.push_back(rotation1[1]);
    axes.push_back(rotation1[2]);
    // Box2 axes
    axes.push_back(rotation2[0]);
    axes.push_back(rotation2[1]);
    axes.push_back(rotation2[2]);

    float minOverlap = FLT_MAX;
    glm::vec3 collisionNormal;

    // Check overlap along each axis
    for (const auto& axis : axes) {
        // Project centers and extents onto axis
        float c1 = glm::dot(center1, axis);
        float c2 = glm::dot(center2, axis);

        // Project extents
        float e1 = glm::abs(glm::dot(extents1 * rotation1[0], axis)) +
                   glm::abs(glm::dot(extents1 * rotation1[1], axis)) +
                   glm::abs(glm::dot(extents1 * rotation1[2], axis));

        float e2 = glm::abs(glm::dot(extents2 * rotation2[0], axis)) +
                   glm::abs(glm::dot(extents2 * rotation2[1], axis)) +
                   glm::abs(glm::dot(extents2 * rotation2[2], axis));

        float overlap = e1 + e2 - glm::abs(c2 - c1);

        if (overlap <= 0) {
            point.collided = false;
            return point;
        }

        if (overlap < minOverlap) {
            minOverlap = overlap;
            collisionNormal = axis * (c2 - c1 < 0 ? -1.0f : 1.0f);
        }
    }

    point.collided = true;
    point.normal = glm::normalize(collisionNormal);
    point.length = minOverlap;

    // Calculate contact points
    point.a = center1 + point.normal * (point.length * 0.5f);
    point.b = center2 - point.normal * (point.length * 0.5f);
    point.point = (point.a + point.b) * 0.5f;

    return point;
}


CollisionPoint CollisionDispatcher::Dispatch(const Collider* collider, const Transform* transform, const Collider* otherCollider, const Transform* otherTransform)
{
    auto typeColliderA = static_cast<int>(collider->GetType());
    auto typeColliderB = static_cast<int>(otherCollider->GetType());

    if (typeColliderA > typeColliderB)
    {
        std::swap(collider, otherCollider);
        std::swap(transform, otherTransform);
    }

    CollisionDispatchFunction collisionDispatchFunction = CollisionDispatcher::collisionDispatcher[typeColliderA][typeColliderB];
    if (collisionDispatchFunction)
    {
#ifdef PHYSICS_DEBUG
        std::cout << "Found resolving function: " << typeColliderA << " " << typeColliderB << std::endl;
#endif
        return collisionDispatchFunction(collider, transform, otherCollider, otherTransform);
    }
    std::cout << "Did not find function: " << typeColliderA << " " << typeColliderB << std::endl;
    return {};
}

// Dispatch matrix definition
// This dispatch matrix is used fot retrieving the correct function for resolving the Collision
CollisionDispatchFunction CollisionDispatcher::collisionDispatcher[static_cast<int>(Collider::ColliderType::COUNT)][static_cast<int>(Collider::ColliderType::COUNT)] = {
    {CollisionDispatcher::IntersectBoxBox, CollisionDispatcher::IntersectBoxSphere},
    {CollisionDispatcher::IntersectBoxSphere, CollisionDispatcher::IntersectSphereSphere}
};



