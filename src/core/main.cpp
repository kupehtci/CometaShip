//
//// #define GLFW_INCLUDE_VULKAN
//

#include "../core/Application.h"

#include <filesystem>
#include <iostream>

#include "../debug/Assertion.h"

//#include "layer_system/Onion.h"
//#include "layer_system/Layer.h"
//
//#include "types/SparseSet.h"
#include "world/Components.h"
#include "world/Entity.h"
#include "world/ComponentRegistry.h"

int main() {

    Application* app = Application::GetInstancePtr();
    app->Init();
    app->Running();
    app->Close();

    delete app;
    
    return 0;
}

