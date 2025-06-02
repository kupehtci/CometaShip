#ifndef COMETA_INPUT_H
#define COMETA_INPUT_H

#include "../core/Singleton.h"
#include "../render/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

enum InputType{
    KEY_PRESSED = 1,
    KEY_RELEASED = 2,
    KEY_HOLD = 3, // ¿?
    MOUSE_MOVED = 4,
    MOUSE_BUTTON_PRESSED = 5,
    MOUSE_BUTTON_RELEASED = 6, 
    MOUSE_SCROLL = 7,
};


class Input : public SingletonManager<Input>{
private:
    // std::unordered_map<int, int> _keys;
    float _xpos, _ypos = 0.0f;
    float _xDeltaPos, _yDeltaPos = 0.0f;

    enum CursorMode
    {
        CURSOR_MODE_NONE = 0,
        CURSOR_MODE_ENABLED = 1,
        CURSOR_MODE_DISABLED = 2,
        CURSOR_MODE_HIDDEN = 4,
        CURSOR_MODE_LOCKED = 8
    };
    CursorMode _cursorMode = CURSOR_MODE_NONE;

public: 
   Input();

   void Init() override; 
   void Update() override;
   void Close() override; 

   static bool IsKeyPressed(int keycode);
   static bool IsMouseButtonPressed(int button);
   static bool IsKeyReleased(int keycode);

   static glm::vec2 GetMousePosition();
   static glm::vec2 GetMouseDelta(); 
   static glm::vec2 GetMouseScroll();

   void HandleKey(GLFWwindow* window, int key, int scancode, int action, int mods);
}; 

#endif // COMETA_INPUT_H