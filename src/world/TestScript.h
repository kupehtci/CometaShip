#ifndef COMETA_TEST_SCRIPT_H
#define COMETA_TEST_SCRIPT_H

#include <iostream>
#include "world/BaseScript.h"

#include "debug/Assertion.h"


class TestScript final : public BaseScript {
  private:
    std::string _text = "";

    uint8_t _health = 100;
    uint8_t _maxHealth = 100;
    uint8_t _damage = 10;

  public:
    TestScript() = default;
    explicit TestScript(const std::string& text) {_text = text;}
    TestScript(const TestScript&) = default;

    ~TestScript() override = default;

    void OnInit() override {
      COMETA_MSG("OnInit ", _text);
    }

    void OnUpdate(float deltaTime) override{
      // COMETA_MSG("OnUpdate ", _text, " delta time: ", deltaTime);
    }

    void OnClose() override{
      COMETA_MSG("OnClose ", _text);
    }

    void OnCollisionEnter(Entity* other, Collision* collision) override{
      COMETA_MSG("[TEST SCRIPT] OnCollisionEnter");
      Tag* otherTag = other->GetComponent<Tag>();
      if (otherTag != nullptr && otherTag->GetTag() == "enemy"){
        _health -= _damage;
      }
    }

    void OnCollisionExit(Entity* other, Collision* collision) override{
      COMETA_MSG("[TEST SCRIPT] OnCollisionExit");
    }
};

#endif //COMETA_TEST_SCRIPT_H
