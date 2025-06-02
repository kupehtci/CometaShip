//
// Created by Daniel Laplana Gimeno on 6/5/25.
//

#ifndef COMETA_PHYSICS_MANAGER_H
#define COMETA_PHYSICS_MANAGER_H

#include "core/Singleton.h"

#include "glm.hpp"

class PhysicsManager : public SingletonManager<PhysicsManager> {
private:
    glm::vec3 _gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    float _beta = 0.2f; // Baumgarte stabilization
    bool _onSimulation = true;

public:
    void Init() override;
    void Update() override;
    void Close() override;

    [[nodiscard]] bool IsOnSimulation() const { return _onSimulation; }
    void SetOnSimulation(bool onSimulation) { _onSimulation = onSimulation; }
};

#endif //COMETA_PHYSICS_MANAGER_H
