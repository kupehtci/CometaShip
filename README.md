
# COMETA

Cometa is custom Game Engine Framework built from scratch using OpenGL as the RendererAPI and GLFW + GLAD to handle modern OpenGL. 

This engine is intended to provide a foundation or a base for common game features such as rendering, advanced input handling and scene management. 

### Features

* OpenGL Rendering: Efficient rendering pipeline with integrated and custom GLSL shaders. 
* GLFW for window management: Handle the window creation and input. 
* GLAD for moder OpenGL Loading: Ensures the cross platform compatibility when loading OpenGL functions. 
* Entity Component System or ECS: Modular and efficient instances handling. 
* ImGUI Integration: Integration with in-engine debugging and functionalities using UI.
* Basic Math Library GLM: Integration with commmon math library GLM to perform advanced operations of transforms. 
* Cross-Platform Compatibility: By using Premake5 and different shell scripting it works in Windows and MacOS.

### Current State

Cometa is currently under development. 


### Future improvements

* Make unable to Load material into a MeshRenderable if no shader is loaded. 

