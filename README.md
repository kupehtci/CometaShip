
# COMETA FRAMEWORK

Cometa Framework is a lightweight C++ game development framework built developed using OpenGL (Via GLFW and GLAD) that provides core systems for: 

* Rendering: in combination with Cometa Render, offers a 3D graphics pipeline with lightning and shadow support. 
* Input handling: Multi-device control using keyboard, mouse and gamepad. 
* Entities composition: using an Entity Component system (ECS) Architecture as the foundation, provides a robust system for object composition. 
* Scene management: custom world that allow to build up scenes composed of entities and hot-swap. 
* Physics simulation: Custom physics engine with collision detection adn rigidbody dynamics. 
* Scripting: scripting is integrated into a layered system together with scripting components for entity-level behaviour.
* UI: Immediate mode UI integrating ImGUI that offers a debug inspector with real time component editing that can be customized

### Cross-platform capabilities



### Getting started

As Cometa use other repositories and systems as dependencies and these are handled as git submodules, in order to pull them together clone the repository:
```bash
git clone --recurse-submodules https://github.com/kupehtci/Cometa.git
```

Cometa relies on Premake5 build configuration to CMake language in order to compile for MacOS distributions and to convert into Visual Studio solutions for Windows.
In order to execute the project, you are going to need premake5 in MacOS distributions installed as a command line tool []()

For both systems, premake5 is integrated as a built-in binary, `premake5` for MacOS and `premake5.exe` for Windows.
In the case of MacOS, it will normally indicate that "Apple cannot verify the app to not contain malicious software". To overpass this, go to Setting > Privacy and Security  and in security properties an option for allowing premake5 execution will appear: 

![Security message in MacOS](./diagrams/images/macos_security_premake.png)

After that, you can use the `build.sh` or `build.ps1` scripts to execute the project on MacOS or to generate a Visual Studio solution for Windows.
```bash
# For MacOS compilation
./build.sh -t macos   

# For windows compilation
./build.ps1
```

`Build.sh` can also be used in Windows by using Git Bash, WSL or another CLI tool that allow UNIX script execution: 
```bash
./build.sh -t macos
```



### Dependencies

The project depends on the following modules or external packages: 
* STB_Image
* OpenGL
* GLFW 
* GLAD
* ImGUI

In version 1.1.0, another libraries has been added for model support:
* Assimp

### Current State

Cometa is currently in version v1.1.0, providing an MVP for basic implementations of videogames or educational purposes. 
The project still in development so stay tune for future updates. 


