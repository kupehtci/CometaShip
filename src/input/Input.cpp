#include "Input.h"

#include <imgui.h>
#include <core/Application.h>

// ------------ FUNCTION DECLARATION ------------
/**
 * Callback to handle a key action
 * @param window
 * @param key
 * @param scancode
 * @param action
 * @param mods
 */
void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void HandleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


// ------------ METHODS IMPLEMENTATION ------------

Input::Input(){
    _xpos = _ypos = 0.0f;
    _xDeltaPos = _yDeltaPos = 0.0f;
}

void Input::Init() {
    // set initial positions stored before calculate delta
    GLFWwindow* currentWindow = Renderer::GetInstancePtr()->GetWindow()->GetGlfwWindow();
    double xpos, ypos = 0.0f;
    glfwGetCursorPos(currentWindow, &xpos, &ypos);
    _xpos = static_cast<float>(xpos);
    _ypos = static_cast<float>(ypos);

    // glfwSetKeyCallback(currentWindow, HandleKeyCallback);
    // glfwSetMouseButtonCallback(currentWindow, HandleMouseButtonCallback);


    // Set initial cursor mode to disabled (Same as locked to the window)
    // glfwSetInputMode(currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // _cursorMode = CURSOR_MODE_DISABLED;
}

void Input::Update() {

    glfwPollEvents();

    GLFWwindow* currentWindow = Renderer::GetInstancePtr()->GetWindow()->GetGlfwWindow();
    double xpos, ypos = 0.0f;
    glfwGetCursorPos(currentWindow, &xpos, &ypos);


    // Only update mouse if cursor is locked or disable into this window
    if (_cursorMode != CURSOR_MODE_ENABLED)
    {
        // Calculate delta movement of the mouse using previous frame mouse position
        _xDeltaPos = xpos - _xpos;
        _yDeltaPos = ypos - _ypos;

        _xpos = xpos;
        _ypos = ypos;
    }

    // Process events
}

void Input::Close() {

}

bool Input::IsKeyPressed(int keycode){
    GLFWwindow* currentWindow = Renderer::GetInstancePtr()->GetWindow()->GetGlfwWindow(); 
    return (glfwGetKey(currentWindow, keycode) == GLFW_PRESS); 
}

bool Input::IsKeyReleased(int keycode){
    GLFWwindow* currentWindow = Renderer::GetInstancePtr()->GetWindow()->GetGlfwWindow();
    return (glfwGetKey(currentWindow, keycode) == GLFW_RELEASE);
}

bool Input::IsMouseButtonPressed(int button){
    GLFWwindow* currentWindow = Renderer::GetInstancePtr()->GetWindow()->GetGlfwWindow();
    return (glfwGetMouseButton(currentWindow, button) == GLFW_PRESS);
}

glm::vec2 Input::GetMousePosition(){
    GLFWwindow* currentWindow = Renderer::GetInstancePtr()->GetWindow()->GetGlfwWindow();
    double xpos, ypos;
    glfwGetCursorPos(currentWindow, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

// Under development
glm::vec2 Input::GetMouseScroll(){
    GLFWwindow* currentWindow = Renderer::GetInstancePtr()->GetWindow()->GetGlfwWindow();
    double xpos, ypos;
    //glfwGetScrollOffset(currentWindow, &xpos, &ypos);
    
    return glm::vec2(xpos, ypos);
}



glm::vec2 Input::GetMouseDelta() {
    Input* lInput = Input::GetInstancePtr();

    return glm::vec2(lInput->_xDeltaPos, lInput->_yDeltaPos); 
}


/**
 * Handle a key action triggered.
 * Is called from the key callback so each key action executed, this function gets called
 * @param window current window called from
 * @param key Key that triggered the action
 * @param scancode
 * @param action action over the key that trigerred the action
 * @param mods key modifiers
 */
void Input::HandleKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
    // Check if must close window

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        switch (_cursorMode)
        {
        case CURSOR_MODE_HIDDEN:
        case CURSOR_MODE_DISABLED:
            // Set the cursor to normal and stop delta motion
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            _xDeltaPos = _yDeltaPos = 0.0f;
            _cursorMode = CURSOR_MODE_ENABLED;
            break;

        case CURSOR_MODE_ENABLED:
            Application::GetInstancePtr()->MustClose();

            _cursorMode = CURSOR_MODE_NONE;
            break;

        case CURSOR_MODE_NONE:
            break;
        default:
            COMETA_WARNING("Not implemented cursor change of state");
            break;
        }
    }
    else if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        switch (_cursorMode)
        {
        case CURSOR_MODE_ENABLED:
        case CURSOR_MODE_NONE:

            std::cout << "IO want to capture keyboard: " << io.WantCaptureKeyboard << std::endl;
            std::cout << "IO want to capture mouse: " << io.WantCaptureMouse << std::endl;
            if (!io.WantCaptureKeyboard && !io.WantCaptureMouse)
            {
                // // set the cursor to disabled (locked into window) and return no previous position
                // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                // glfwSetCursorPos(window, _xpos,_ypos );
                // _cursorMode = CURSOR_MODE_DISABLED;
            }
            else
            {
                io.AddKeyEvent(static_cast<ImGuiKey>(key), true);
                io.AddMouseButtonEvent(key, true);
            }
            break;

        case CURSOR_MODE_DISABLED:
        case CURSOR_MODE_HIDDEN:
            break;

        default:
            COMETA_WARNING("Not implemented cursor change of state");
            break;
        }
    }

    // // DEBUG Cursor state
    // std::cout << "Current cursor state: " << _cursorMode  << std::endl;
}

// Key callback executed when GLFW detect a key action under the window
void HandleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Input::GetInstancePtr()->HandleKey(window, key, scancode, action, mods);
    ImGuiIO& io = ImGui::GetIO();

}

void HandleMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    // Input::GetInstancePtr()->HandleKey(window, button, 0, action, mods);
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action == GLFW_PRESS);
}