//
// Created by Daniel Laplana Gimeno on 1/5/25.
//

#ifndef UILAYER_H
#define UILAYER_H

#include "layer_system/Layer.h"

#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <unordered_map>

#include <filesystem>

class UILayer : public Layer{

private:
    bool _mainWindowOpen = true;
    bool _sceneHierarchyOpen = false;
    ImVec2 _thumbnailSize = ImVec2(128, 128);

    std::string currentPath = std::filesystem::current_path().string();

    bool isOnSimulation = false;
    
    // Maps to store script code for each entity
    std::unordered_map<uint32_t, std::string> _entityStartCode;
    std::unordered_map<uint32_t, std::string> _entityUpdateCode;
    std::unordered_map<uint32_t, std::string> _entityCollisionEnterCode;
    std::unordered_map<uint32_t, std::string> _entityCollisionExitCode;
    std::unordered_map<uint32_t, std::string> _entityDestroyCode;

public:
    UILayer();
    ~UILayer() override;

    void Init() override;
    void Update() override;
    void Close() override;

    void HandleEvent(Event& event) override;

    // Custom UI methods
    void BuildSceneHierarchyPanel();
};



#endif //UILAYER_H
