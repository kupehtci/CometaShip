//
// Created by Daniel Laplana Gimeno on 19/1/25.
//
#define GL_SILENCE_DEPRECATION

#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <core/Application.h>
#include <input/Input.h>

#include "render/Renderer.h"
#include "core/Application.h"
#include "input/Input.h"



// ------------ FUNCTION DECLARATION ------------
/**
 * Callback to handle the window resize through OpenGL function
 * @param window Window pointer passed through the OpenGL callback
 * @param width New width passed through the OpenGL callback
 * @param height New height passed through the OpenGL callback
 */
void HandleResizeCallback(GLFWwindow *window, int width, int height);


// Window constructor
Window::Window()
{
    this->_resolution = glm::vec2(COMETA_DEFAULT_WIDTH, COMETA_DEFAULT_HEIGHT);
    this->_window = nullptr;
    this->_title = "none";
}

/**
 * Window destructor
 */
Window::~Window(){
    if(this->_window !=  nullptr){
        glfwDestroyWindow(this->_window);
        // delete _window;
    }

}

void Window::Create(int width, int height, const char *title) {

    // Store resolution parameters
    _resolution = {width, height};
    this->_title = title;


    // Create a GLFW window
    this->_window = glfwCreateWindow(width,height, title, NULL, NULL);

    if (!_window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(_window);

    // Set Callbacks

    glfwSetWindowSizeCallback(_window, HandleResizeCallback);

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Application* app = Application::GetInstancePtr();

        switch (action)
        {
            case GLFW_PRESS:
            {
                KeyPressEvent event = KeyPressEvent(key);
                EventBus::GetInstancePtr()->Notify(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event = KeyReleasedEvent(key);
                EventBus::GetInstancePtr()->Notify(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressEvent event = KeyPressEvent(key, true);
                EventBus::GetInstancePtr()->Notify(event);
                break;
            }
            default:
            {
                COMETA_WARNING("[WINDOW][CREATE] Not specified event for SetKeyCallback()");
                break;
            }
        }
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
    {
        switch (action)
        {
            case GLFW_PRESS:
            {
                MousePressButtonEvent event = MousePressButtonEvent(button);
                EventBus::GetInstancePtr()->Notify(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseReleaseButtonEvent event = MouseReleaseButtonEvent(button);
                EventBus::GetInstancePtr()->Notify(event);
                break;
            }
            default:
            {
                COMETA_WARNING("[WINDOW][CREATE] Not specified event for SetMouseButtonCallback()");
                break;
            }
        }
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double x, double y)
    {
        MouseMoveEvent event = MouseMoveEvent(static_cast<float>(x), static_cast<float>(y));
        EventBus::GetInstancePtr()->Notify(event);
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double offsetX, double offsetY)
    {
        MouseScrollEvent event = MouseScrollEvent(static_cast<float>(offsetX), static_cast<float>(offsetY));
        EventBus::GetInstancePtr()->Notify(event);
    });
}

void Window::Init() {

}


void Window::Update() {
    // Render();
}


void Window::SwapBuffers() {

//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear the screen

    glfwSwapBuffers(_window);
    glfwPollEvents();
    glEnable(GL_DEPTH_TEST); 
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear the screen
}




bool Window::ShouldHandleCloseWindow(){
    return glfwWindowShouldClose(this->_window);
}

/**
 * Close the window and clean it
 * Also clean the rest of parameters of the window
 */
void Window::Close() {

    if(this->_window !=  nullptr){
        glfwDestroyWindow(this->_window);
    }

    COMETA_ASSERT(("Window " + (std::string)this->_title +  " closed correctly").c_str());
}

/**
 * Handle the resize of the window
 */
void Window::HandleResize(GLFWwindow* window, int width, int height) {


    // Use for debugging
    glm::vec2 previousResolution = _resolution;

    // glfwGetWindowSize(_window, &_resolution->x, &_resolution->y);
    glm::ivec2 currentFramebufferSize = {};
    glfwGetFramebufferSize(_window, &currentFramebufferSize.x, &currentFramebufferSize.y);
    
    // COMETA_ASSERT(("Handling resize from " + std::to_string(previousResolution.x)  + ", " + std::to_string(previousResolution.y) + " to " + std::to_string(_resolution->x) + ", " + std::to_string(_resolution->y)).c_str());

    // modify viewport resolution
    glViewport( 0.f, 0.f, currentFramebufferSize.x, currentFramebufferSize.y);

    _resolution = glm::vec2(currentFramebufferSize.x, currentFramebufferSize.y);
}


/**
 * Callback that is called from GLFW library and calls the Window HandleResize method to handle the resize of the window
 * This function is called from OpenGL as a callback
 * @param window (GLFWwindow*) window pointer of the main window that is being resized
 * @param width (int) new width value of the window
 * @param height (int) new height value of the window
 */
void HandleResizeCallback(GLFWwindow* window, int width, int height){
    Window::GetInstancePtr()->HandleResize(window, width, height);
}
