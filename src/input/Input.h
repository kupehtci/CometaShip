#ifndef COMETA_INPUT_H
#define COMETA_INPUT_H

#include "../core/Singleton.h"
#include "../render/Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

// Include custom keys
#include "input/CometaKeys.h"
#include "input/CometaJoystick.h"
#include "input/CometaGamepadInfo.h"

/**
 * Types of inputs available
 */
enum InputType{
    KEY_PRESSED = 1,
    KEY_RELEASED = 2,
    KEY_HOLD = 3, // ¿?
    MOUSE_MOVED = 4,
    MOUSE_BUTTON_PRESSED = 5,
    MOUSE_BUTTON_RELEASED = 6, 
    MOUSE_SCROLL = 7,
};

/**
 * Input manager is the singleton class in charge of input handling
 */
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

    // Joystick methods

    /**
     * Check if a specific joystick or gamepad is connected
     * @param joystick CometaJoystick index of the joystick to check
     * @return true if its connected, false otherwise
     */
    [[nodiscard]] static bool IsJoystickConnected(CometaJoystick joystick) {return glfwJoystickPresent(joystick) == GLFW_TRUE;}


    /**
     * Check wether the joystick is a valid gamepad
     * @param joystick Joystick to check is its a gamepad
     * @return True if joystick is a valid gamepad
     */
    [[nodiscard]] static bool IsJoystickAGamepad(CometaJoystick joystick) {return glfwJoystickIsGamepad(joystick) == GLFW_TRUE;}

    [[nodiscard]] static CometaGamepadInfo GetGamepadInfo(CometaJoystick joystick)
    {
        CometaGamepadInfo info{};
        GLFWgamepadstate state{};
        glfwGetGamepadState(joystick, &state);

        for (int i = 0; i < 15; i++)
        {
            info.buttons[i] = state.buttons[i];
        }
        info.axes[0] = state.axes[0];
        info.axes[1] = state.axes[1];
        info.axes[2] = state.axes[2];
        info.axes[3] = state.axes[3];
        info.axes[4] = state.axes[4];
        info.axes[5] = state.axes[5];
        return info;
    }

    /**
     * Retrieve the axis of an specific joystick or gamepad.
     * @param joystick CometaJoystick Joystick index
     * @param axisCount Passed as reference return the maximum number of axis of the gamepad
     * @return float array with the axis
     */
    [[nodiscard]] static const float* GetJoystickAxes(CometaJoystick joystick, int& axisCount) {return glfwGetJoystickAxes(joystick, &axisCount); }
}; 

#endif // COMETA_INPUT_H