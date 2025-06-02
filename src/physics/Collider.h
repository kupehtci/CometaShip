//
// Created by Daniel Laplana Gimeno on 6/5/25.
//

#ifndef COMETA_COLLIDER_H
#define COMETA_COLLIDER_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/matrix_transform.hpp>
// #include <world/Components.h>

#include "physics/CollisionDispatcher.h"


// Base Collider class (abstract)
class Collider {
public:
    enum class ColliderType {
        BOX_COLLIDER = 0,
        SPHERE_COLLIDER = 1,
        COUNT = 2,
    };

    virtual ~Collider() = default;
    [[nodiscard]] virtual ColliderType GetType() const = 0;
    virtual void DebugDraw() const = 0;

    virtual glm::mat3 CalculateInertiaTensor(float mass) = 0;
    virtual glm::mat3 CalculateInverseInertiaTensor(float mass) = 0;

protected:
    Collider() = default;
};



// Box Collider implementation
class BoxCollider : public Collider {
private:
    glm::vec3 _extents = { 0.5f, 0.5f, 0.5f };
    glm::vec3 _center = { 0.0f, 0.0f, 0.0f };
    glm::quat _rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);


public:
    BoxCollider() = default;

    explicit BoxCollider(const glm::vec3& extents)
        : _extents(extents), _center({0.0f, 0.0f, 0.0f}) {}

    BoxCollider(const glm::vec3& extents, const glm::vec3& center)
    : _extents(extents), _center(center) {}

    BoxCollider(const glm::vec3& extents, const glm::vec3& center, const glm::quat& rotation)
        : _extents(extents), _center(center), _rotation(rotation) {}

    [[nodiscard]] ColliderType GetType() const override { return ColliderType::BOX_COLLIDER; }

    void DebugDraw() const override {
        // Use DebugRenderer to visualize this box collider
     
    }

    glm::mat3 CalculateInertiaTensor(float mass) override
    {
        // Calculate dimensions
        glm::vec3 size = GetSize();

        // Calculate local inertia tensor for a box
        float x2 = size.x * size.x;
        float y2 = size.y * size.y;
        float z2 = size.z * size.z;
        glm::mat3 inertiaTensor {};
        // Initialize with identity matrix
        inertiaTensor = glm::mat3(1.0f);

        // Set diagonal elements (mass will be multiplied later)
        inertiaTensor[0][0] = (y2 + z2) / 12.0f;  // Ixx
        inertiaTensor[1][1] = (x2 + z2) / 12.0f;  // Iyy
        inertiaTensor[2][2] = (x2 + y2) / 12.0f;  // Izz

        return inertiaTensor;
    }

    glm::mat3 CalculateInverseInertiaTensor(float mass) override
    {
        glm::vec3 size = GetSize();

        float x2 = size.x * size.x;
        float y2 = size.y * size.y;
        float z2 = size.z * size.z;
        glm::mat3 inertiaTensor = glm::mat3(1.0f);
        inertiaTensor[0][0] = (y2 + z2) / 12.0f;
        inertiaTensor[1][1] = (x2 + z2) / 12.0f;
        inertiaTensor[2][2] = (x2 + y2) / 12.0f;

        return glm::inverse(inertiaTensor);
    }

    // Getters and setters
    [[nodiscard]] const glm::vec3& GetExtents() const { return _extents; }
    [[nodiscard]] glm::vec3 GetSize() const { return _extents * 2.0f; }
    [[nodiscard]] const glm::vec3& GetCenter() const { return _center; }
    [[nodiscard]] const glm::quat& GetRotation() const { return _rotation; }

    void SetExtents(const glm::vec3& extents) { _extents = extents; }
    void SetCenter(const glm::vec3& center) { _center = center; }
    void SetRotation(const glm::quat& rotation) { _rotation = rotation; }
};

// Sphere Collider implementation
class SphereCollider : public Collider {
private:
    float _radius = 0.5f;
    glm::vec3 _center = { 0.0f, 0.0f, 0.0f };

public:
    SphereCollider() = default;
    explicit SphereCollider(float radius)
        : _radius(radius), _center({0.0f, 0.0f, 0.0f}) {}
    SphereCollider(float radius, const glm::vec3& center)
        : _radius(radius), _center(center) {}

    [[nodiscard]] ColliderType GetType() const override { return ColliderType::SPHERE_COLLIDER; }

    void DebugDraw() const override {
        // Use DebugRenderer to visualize this sphere collider
        
    }


    glm::mat3 CalculateInertiaTensor(float mass) override
    {
        // Calculate inertia tensor for a solid sphere
        float i = (2.0f * _radius * _radius) / 5.0f;

        glm::mat3 inertiaTensor = glm::mat3(
            i, 0.0f, 0.0f,
            0.0f, i, 0.0f,
            0.0f, 0.0f, i
        );
        return inertiaTensor;
    }

    glm::mat3 CalculateInverseInertiaTensor(float mass) override
    {
        float i = (2.0f * _radius * _radius) / 5.0f;

        glm::mat3 inverseInertiaTensor {};
        inverseInertiaTensor = glm::mat3(
            1.0f/i, 0.0f, 0.0f,
            0.0f, 1.0f/i, 0.0f,
            0.0f, 0.0f, 1.0f/i
        );
        return inverseInertiaTensor;
    }


    // Getters and setters
    float GetRadius() const { return _radius; }
    const glm::vec3& GetCenter() const { return _center; }

    void SetRadius(float radius) { _radius = radius; }
    void SetCenter(const glm::vec3& center) { _center = center; }
};


#endif //COMETA_COLLIDER_H
