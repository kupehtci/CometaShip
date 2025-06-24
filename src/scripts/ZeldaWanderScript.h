#include "world/BaseScript.h"
#include "world/Entity.h"
#include <glm.hpp>
#include <random>

class ZeldaWanderScript : public BaseScript {
public:
    ZeldaWanderScript(Entity* owner, float maxDistance);
    void OnInit() override;
    void OnUpdate(float dt) override;
    void OnClose() override;
    void OnCollisionEnter(Entity* other, Collision* collision) override;
    void OnCollisionExit(Entity* other, Collision* collision) override;
private:
    glm::vec3 _center;
    float _maxDistance;
    float _speed;
    glm::vec3 _direction;
    glm::vec3 _target;
    float _waitTimer;
    float _changeDirTimer;
    std::mt19937 _rng;
    std::uniform_real_distribution<float> _angleDist;
};
