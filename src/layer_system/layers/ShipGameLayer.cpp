#include <memory>
#include <algorithm>

#include "ShipGameLayer.h"

#include <layer_system/EventBus.h>
#include <physics/Collider.h>

#include "render/Renderer.h"
#include "render/Shader.h"
#include "render/Mesh.h"

#include "world/Entity.h"
#include "world/WorldManager.h"
#include "world/Components.h"

#include "input/Input.h"

#include "core/Time.h"

// Include our custom scripts
#include "world/ShipScript.h"
#include "world/ObstacleScript.h"

// Include ImGui for menu rendering
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"


ShipGameLayer::ShipGameLayer() {
    _name = "ShipGameLayer";
}

ShipGameLayer::~ShipGameLayer() {
    // Cleanup will be handled in Close()
}

void ShipGameLayer::Init() {
    // Set up camera
    _camera = Camera();
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, -5.0f);
    _camera.GetPosition() = cameraPos;
    _camera.OnUpdate();

    WorldManagerRef->CreateWorld(0);
    WorldManagerRef->SetCurrentWorld(0);
    std::shared_ptr<World> gameWorld = WorldManagerRef->GetWorld(0);
    gameWorld->SetCamera(&_camera);

    InitImGui();
    
    // Initialize game world but don't start gameplay yet
    InitializeGameWorld();
    InitializeObstaclePool();

    // Subscribe to input events
    EventBus::GetInstancePtr()->Subscribe(EventType::COMETA_KEY_PRESS_EVENT, this);
    EventBus::GetInstancePtr()->Subscribe(EventType::COMETA_KEY_RELEASE_EVENT, this);
}

void ShipGameLayer::InitImGui() {
     std::cout << "UILayer::Init" << std::endl;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Renderer::GetInstancePtr()->GetWindow()->GetGlfwWindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // Subscribe to events
    EventBus::GetInstancePtr()->Subscribe(COMETA_KEY_PRESS_EVENT, this);


    ImGuiStyle* style = &ImGui::GetStyle();

    // Fonst assign
    float fontSize = 15.0f;
    io.Fonts->AddFontFromFileTTF("resources/Fonts/jetbrains/JetBrainsMonoNL-Regular.ttf", fontSize);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/Fonts/jetbrains/JetBrainsMonoNL-Regular.ttf", fontSize);

    style->WindowPadding = ImVec2(15, 15);
    style->WindowRounding = 5.0f;
    style->FramePadding = ImVec2(5, 5);
    style->FrameRounding = 4.0f;
    style->ItemSpacing = ImVec2(12, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->IndentSpacing = 25.0f;
    style->ScrollbarSize = 15.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->ScaleAllSizes(0.3f);

    //style->ChildRounding = 3.0f; 
    //style->ChildBorderSize = 15.0f;



    style->Colors[ImGuiCol_Text] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.39f, 0.38f, 0.77f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.92f, 0.91f, 0.88f, 0.70f);
    // style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.58f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.92f, 0.91f, 0.88f, 0.92f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.84f, 0.83f, 0.80f, 0.65f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.99f, 1.00f, 0.40f, 0.78f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.47f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.21f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.90f, 0.91f, 0.00f, 0.78f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    // style->Colors[ImGuiCol_ComboBg] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 1.00f, 0.00f, 0.80f);
    
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);

    style->Colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 0.14f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.99f, 1.00f, 0.22f, 0.86f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);

    style->Colors[ImGuiCol_Header] = ImVec4(0.655f, 0.627f, 0.741f, 0.76f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.655f, 0.627f, 0.741f, 0.86f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.655f, 0.627f, 0.741f, 1.00f);

    // style->Colors[ImGuiCol_Column] = ImVec4(0.00f, 0.00f, 0.00f, 0.32f);
    /*style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
    style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);*/
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.04f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 1.00f, 0.00f, 0.78f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
   /* style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);*/
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    //style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

}


void ShipGameLayer::InitializeGameWorld() {
    // Set up camera
    _camera = Camera();
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 4.0f);
    _camera.SetPosition(cameraPos);
    _camera.OnUpdate();


    WorldManagerRef->CreateWorld(0);
    WorldManagerRef->SetCurrentWorld(0);
    std::shared_ptr<World> gameWorld = WorldManagerRef->GetWorld(0);
    gameWorld->SetCamera(&_camera);
    
    _playerShip = gameWorld->CreateEntity("PlayerShip");
    _playerShipId = _playerShip->GetUID();
    

    Transform* shipTransform = _playerShip->GetComponent<Transform>();
    shipTransform->position = _playerShipPosition;
    shipTransform->scale = glm::vec3(0.2f, 0.2f, 0.2f);
    
    // Add collider to player ship
    ColliderComponent* shipCollider = _playerShip->CreateComponent<ColliderComponent>();
    shipCollider->SetCollider<BoxCollider>(glm::vec3(0.2f, 0.8f, 0.4f)); 

    // Add rigidbody to player ship
    RigidBody* shipRb = _playerShip->CreateComponent<RigidBody>();
    shipRb->SetMass(1.0f);
    shipRb->SetAffectedByGravity(false);

    // Set the player ship's renderable component with rocket loaded model
    auto playerShipRenderable = _playerShip->CreateComponent<MeshRenderable>();
    playerShipRenderable->LoadModel("resources/models/rocket/miiWeaponShell11.fbx");

    
    // Add script component to player ship
    Script* shipScript = _playerShip->CreateComponent<Script>();
    shipScript->Attach<ShipScript>();


    // Add tag component to identify the player
    Tag* shipTag = _playerShip->CreateComponent<Tag>();
    shipTag->SetTag("player");
    
    // Create a directional light
    Entity* lightEntity = gameWorld->CreateEntity("DirectionalLight");
    DirectionalLight* dirLight = lightEntity->CreateComponent<DirectionalLight>();
    dirLight->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
    
    // Create a floor/background
    Entity* floor = gameWorld->CreateEntity("Floor");
    floor->GetComponent<Transform>()->position = glm::vec3(0.0f, -4.0f, -15.0f);
    floor->GetComponent<Transform>()->rotation = glm::vec3(90.0f, 0.0f, 0.0f);
    floor->GetComponent<Transform>()->scale = glm::vec3(20.0f, 0.1f, 40.0f);
    
    MeshRenderable* floorRenderable = floor->CreateComponent<MeshRenderable>();
    floorRenderable->SetMesh(Mesh::CreateBox());
    
    std::shared_ptr<Material> floorMaterial = std::make_shared<Material>(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        8.0f,
        "resources/space.jpg",
        "resources/black.jpg",
        "resources/black.jpg"
    );
    
    floorMaterial->LoadShader("Floor Shader", 
        "src/render/shaders/blinn_phong_shader.vert", 
        "src/render/shaders/blinn_phong_shader.frag");
    
    floorRenderable->SetMaterial(floorMaterial);
    
    // --- POINT LIGHT ---
    Entity* ptlight0 = gameWorld->CreateEntity("Light Point 1");
    ptlight0->CreateComponent<PointLight>();

    MeshRenderable* ptlight0Renderable = ptlight0->CreateComponent<MeshRenderable>();

    std::shared_ptr<Material> material1 = std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f),
                                    glm::vec3(1.0f, 0.5f, 0.31f),
                                    glm::vec3(1.0f, 0.5f, 0.31f),
                                    glm::vec3(0.5f, 0.5f, 0.5f),
                                    2.0f,
                                    "resources/white.jpg",
                                    "resources/white.jpg",
                                    "resources/black.jpg");

    material1->LoadShader("Main Shader","src/render/shaders/light_shader.vert", "src/render/shaders/light_shader.frag");

    ptlight0Renderable->SetMesh(Mesh::CreateSphere());
    ptlight0Renderable->SetMaterial(material1);
    ptlight0->GetComponent<Transform>()->position = glm::vec3(0.0f, 1.0f, 5.0f);
    ptlight0->GetComponent<Transform>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);
    // --- END OF POINT LIGHT --- 

    // Subscribe to input events
    EventBus::GetInstancePtr()->Subscribe(EventType::COMETA_KEY_PRESS_EVENT, this);
    EventBus::GetInstancePtr()->Subscribe(EventType::COMETA_KEY_RELEASE_EVENT, this);

}


void ShipGameLayer::Update() {
    _camera.OnUpdate();
    float deltaTime = Time::GetDeltaTime();
    UpdateObstacles(deltaTime);



    // --- Score popup/delayed increment logic ---
    static float _scoreDelayTimer = 0.0f;
    _scoreDelayTimer += deltaTime;
    if (!_showScorePopup && _scoreDelayTimer >= 2.0f) {
        _showScorePopup = true;
        _scorePopupTimer = 1.0f;
    }
    if (_showScorePopup) {
        _scorePopupTimer -= deltaTime;
        if (_scorePopupTimer <= 0.0f) {
            UpdateScore(10);
            _showScorePopup = false;
            _scoreDelayTimer = 0.0f;
        }
    }
    // --- End of score popup logic ---

    switch (_currentState) {
        case GameState::MENU:
            RenderMenu();
            break;
        case GameState::PLAYING:
            if (_gameRunning) {

                // Update obstacles spawn timer
                _obstacleSpawnTimer += deltaTime;
                if (_obstacleSpawnTimer >= _obstacleSpawnInterval) {
                    SpawnObstacle();
                    _obstacleSpawnTimer = 0.0f;
                    _gameSpeed += 0.001f;
                    _obstacleSpawnInterval = std::max(0.5f, _obstacleSpawnInterval - 0.02f);
                }
            }

            // --- Check for Game Over ---
            if (_playerShip) {
                auto shipScriptComponent = _playerShip->GetComponent<Script>();
                if (shipScriptComponent) {
                    auto shipScript = std::dynamic_pointer_cast<ShipScript>(shipScriptComponent->GetScript());
                    if (shipScript && (!shipScript->IsAlive() || shipScript->GetLives() == 0)) {
                        _currentState = GameState::GAME_OVER;
                        _gameRunning = false;
                    }
                }
            }
            // --- End Game Over check ---

            // Render HUD overlay
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            RenderGameplayHUD();
            ImGui::EndFrame();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            break;
        case GameState::PAUSED:
            RenderPauseMenu();
            break;
        case GameState::GAME_OVER:
            RenderGameOverScreen();
            break;
    }
}

void ShipGameLayer::RenderMenu() {
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(300, 400));
    
    ImGui::Begin("Cometa Ship", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    
    ImGui::Text("Main Menu");
    ImGui::Separator();
    
    if (ImGui::Button("Start Game", ImVec2(280, 40))) {
        StartGame();
    }
    
    if (ImGui::Button("Controls", ImVec2(280, 40))) {
        _showControls = true;
    }
    
    if (ImGui::Button("High Scores", ImVec2(280, 40))) {
        _showHighScores = true;
    }
    
    if (ImGui::Button("Exit", ImVec2(280, 40))) {
        ExitGame();
    }
    
    if (_showControls) {
        ImGui::Begin("Controls", &_showControls);
        ImGui::Text("A/Left Arrow - Move Left");
        ImGui::Text("D/Right Arrow - Move Right");
        ImGui::Text("P - Pause Game");
        ImGui::Text("R - Reset Game");
        ImGui::End();
    }
    
    if (_showHighScores) {
        ImGui::Begin("High Scores", &_showHighScores);
        ImGui::Text("Coming Soon!");
        ImGui::End();
    }
    
    ImGui::End();
    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShipGameLayer::RenderPauseMenu() {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    
    ImGui::Begin("Pause Menu", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("Game Paused");
    
    if (ImGui::Button("Resume")) {
        _currentState = GameState::PLAYING;
        _gameRunning = true;
    }
    
    if (ImGui::Button("Return to Main Menu")) {
        _currentState = GameState::MENU;
    }
    
    ImGui::End();
    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShipGameLayer::RenderGameOverScreen() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_Always);
    ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    ImGui::Text("\n   GAME OVER!");
    ImGui::Separator();
    ImGui::Text("Final Score: %d", _score);
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("What would you like to do?");
    ImGui::Spacing();
    if (ImGui::Button("Try Again", ImVec2(320, 40))) {
        auto shipScriptComponent = _playerShip->GetComponent<Script>();
        auto shipScript = std::dynamic_pointer_cast<ShipScript>(shipScriptComponent->GetScript());
        if (shipScript) {
            shipScript->ResetLives();
        }

        ResetGame();
        ResetScore();
        _currentState = GameState::PLAYING;
    }
    if (ImGui::Button("Return to Main Menu", ImVec2(320, 40))) {
        
        auto shipScriptComponent = _playerShip->GetComponent<Script>();
        auto shipScript = std::dynamic_pointer_cast<ShipScript>(shipScriptComponent->GetScript());
        if (shipScript) {
            shipScript->ResetLives();
        }
        ResetGame();
        ResetScore();

        _currentState = GameState::MENU;
    }

    ImGui::Spacing();
    ImGui::Text("Press ESC to exit the game.");
    ImGui::End();
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShipGameLayer::RenderGameplayHUD() {
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(1.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White background
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); // Dark text
    ImGui::Begin("HUD", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

    auto shipScriptComponent = _playerShip->GetComponent<Script>(); 
    auto shipScript = std::dynamic_pointer_cast<ShipScript>(shipScriptComponent->GetScript());
    if(shipScript) {
        _playerHealth = shipScript->GetLives();
    }

    ImGui::Text("Score: %d", _score);
    if (_showScorePopup) {
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.0f, 0.6f, 0.0f, 1.0f), "+10");
    }
    ImGui::Separator();
    ImGui::Text("Health: %d", _playerHealth);
    ImGui::End();
    ImGui::PopStyleColor(2);
}

void ShipGameLayer::StartGame() {
    _currentState = GameState::PLAYING;
    _gameRunning = true;
    ResetGame();
}

void ShipGameLayer::ExitGame() {
    Close();
}

void ShipGameLayer::HandleEvent(Event& event) {
    if (event.GetEventType() == COMETA_KEY_PRESS_EVENT) {
        KeyPressEvent& keyEvent = dynamic_cast<KeyPressEvent&>(event);
        int key = keyEvent.GetKey();

        switch (_currentState) {
            case GameState::PLAYING:
                if (key == GLFW_KEY_R) {
                    ResetGame();
                    event.SetHandled();
                } else if (key == GLFW_KEY_P) {
                    _currentState = GameState::PAUSED;
                    _gameRunning = false;
                    event.SetHandled();
                }
                break;
                
            case GameState::MENU:
                HandleMenuInput(key);
                break;
                
            case GameState::PAUSED:
                if (key == GLFW_KEY_P) {
                    _currentState = GameState::PLAYING;
                    _gameRunning = true;
                    event.SetHandled();
                }
                break;
        }
    }
}

void ShipGameLayer::HandleMenuInput(int key) {
    switch (key) {
        case GLFW_KEY_UP:
            _selectedMenuItem = (_selectedMenuItem - 1 + NUM_MENU_ITEMS) % NUM_MENU_ITEMS;
            break;
            
        case GLFW_KEY_DOWN:
            _selectedMenuItem = (_selectedMenuItem + 1) % NUM_MENU_ITEMS;
            break;
            
        case GLFW_KEY_ENTER:
            switch (_selectedMenuItem) {
                case 0: StartGame(); break;
                case 1: ShowControls(); break;
                case 2: ShowHighScores(); break;
                case 3: ExitGame(); break;
            }
            break;
    }
}

void ShipGameLayer::Close() {
    _gameRunning = false;
}

/**
 * Initialize the obstacle pool with a fixed number of obstacles
 * This creates a pool of inactive obstacles that can be reused
 * when new obstacles are needed
 */
void ShipGameLayer::InitializeObstaclePool() {
    
    std::shared_ptr<World> gameWorld = WorldManagerRef->GetCurrentWorld();
    _obstaclePool.clear();
    _activeObstacles.clear();

    for (size_t i = 0; i < _obstaclePoolSize; ++i) {
        Entity* obstacle = gameWorld->CreateEntity("ObstaclePool_" + std::to_string(i));
        Transform* obstacleTransform = obstacle->GetComponent<Transform>();
        obstacleTransform->position = glm::vec3(1000.0f, 1000.0f, 1000.0f); // Move off-screen
        obstacleTransform->scale = glm::vec3(1.0f);

        ColliderComponent* obstacleCollider = obstacle->CreateComponent<ColliderComponent>();
        obstacleCollider->SetCollider<BoxCollider>(obstacleTransform->scale);
        RigidBody* obstacleRb = obstacle->CreateComponent<RigidBody>();

        obstacleRb->SetAffectedByGravity(false);
        obstacleRb->SetMass(1.0f);
        obstacleRb->SetLinearVelocity(glm::vec3(0.0f));
        MeshRenderable* obstacleRenderable = obstacle->CreateComponent<MeshRenderable>();

        // std::shared_ptr<Material> obstacleMaterial = std::make_shared<Material>(
        //     glm::vec3(1.0f, 1.0f, 1.0f),
        //     glm::vec3(0.8f, 0.1f, 0.1f),
        //     glm::vec3(0.9f, 0.2f, 0.2f),
        //     glm::vec3(1.0f, 0.6f, 0.6f),
        //     16.0f,
        //     "resources/white.jpg",
        //     "resources/white.jpg",
        //     "resources/black.jpg"
        // );
        // obstacleMaterial->LoadShader("Obstacle Shader", 
        //     "src/render/shaders/blinn_phong_shader.vert", 
        //     "src/render/shaders/blinn_phong_shader.frag");
            
        // obstacleRenderable->SetMaterial(obstacleMaterial);
        // obstacleRenderable->SetMesh(Mesh::CreateBox());
        obstacleRenderable->LoadModel("resources/models/Rock/ObjRock.fbx");

        Script* obstacleScript = obstacle->CreateComponent<Script>();
        obstacleScript->Attach<ObstacleScript>(_gameSpeed);
        Tag* obstacleTag = obstacle->CreateComponent<Tag>();
        obstacleTag->SetTag("obstacle");
        // Add a custom flag to mark as inactive
        obstacle->SetName("inactive");
        _obstaclePool.push_back(obstacle);
    }
}

Entity* ShipGameLayer::GetPooledObstacle() {
    if (_obstaclePool.empty()) return nullptr;
    Entity* obstacle = _obstaclePool.back();
    _obstaclePool.pop_back();
    _activeObstacles.push_back(obstacle);
    obstacle->SetName("active");
    return obstacle;
}

/**
 * Deactivate an obstacle by moving it out of screen and resetting its state
 * This allows the obstacle to be reused later in the pool
 */
void ShipGameLayer::DeactivateObstacle(Entity* obstacle) {
    if (!obstacle) return;
    // Move outside the screen and reset velocity
    Transform* t = obstacle->GetComponent<Transform>();
    if (t) t->position = glm::vec3(1000.0f, 1000.0f, 1000.0f);
    RigidBody* rb = obstacle->GetComponent<RigidBody>();
    if (rb) rb->SetLinearVelocity(glm::vec3(0.0f));

    // Mark as inactive
    obstacle->SetName("inactive");
    
    // Remove from active list
    auto it = std::find(_activeObstacles.begin(), _activeObstacles.end(), obstacle);
    if (it != _activeObstacles.end()) _activeObstacles.erase(it);
    _obstaclePool.push_back(obstacle);
}


void ShipGameLayer::UpdateObstacles(float deltaTime) {
    for (auto it = _activeObstacles.begin(); it != _activeObstacles.end(); ) {
        Entity* obstacle = *it;
        bool deactivate = false;
        Transform* t = obstacle->GetComponent<Transform>();
        if (t && t->position.y < -20.0f) deactivate = true;

        // Check lifespan via ObstacleScript
        Script* script = obstacle->GetComponent<Script>();
        if (script) {
            auto obsScript = std::dynamic_pointer_cast<ObstacleScript>(script->GetScript());
            if (obsScript && obsScript->IsExpired()) deactivate = true;
        }
        if (deactivate) {
            DeactivateObstacle(obstacle);
            it = _activeObstacles.begin(); 
        } else {
            ++it;
        }
    }
}

void ShipGameLayer::ResetGame() {
    _gameRunning = true;
    // _score = 0;
    _obstacleSpawnTimer = 0.0f;
    _obstacleSpawnInterval = 2.0f;
    _gameSpeed = 5.0f;

    std::shared_ptr<World> gameWorld = WorldManagerRef->GetCurrentWorld();

    // Deactivate all obstacles
    for (Entity* obstacle : _activeObstacles) {
        DeactivateObstacle(obstacle);
    }

    _activeObstacles.clear();

    
    if (_playerShip) {
        Transform* shipTransform = _playerShip->GetComponent<Transform>();
        shipTransform->position = _playerShipPosition; 
        shipTransform->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

        // Reset physics
        RigidBody* shipRb = _playerShip->GetComponent<RigidBody>();
        if (shipRb) {
            shipRb->SetLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
            shipRb->SetAffectedByGravity(false);
            shipRb->SetMass(1.0f);
            shipRb->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        }

        // Reset script state
        Script* shipScript = _playerShip->GetComponent<Script>();
        if (shipScript) {
            auto shipScriptInstance = std::dynamic_pointer_cast<ShipScript>(shipScript->GetScript());
            if (shipScriptInstance) {
                shipScriptInstance->SetAlive(true);
            }
        }

        // Reset visual appearance
        MeshRenderable* renderable = _playerShip->GetComponent<MeshRenderable>();
        if (renderable && renderable->GetMaterial()) {
            renderable->GetMaterial()->SetAmbient(glm::vec3(0.1f, 0.1f, 0.8f));
            renderable->GetMaterial()->SetDiffuse(glm::vec3(0.2f, 0.2f, 0.9f));
        }
    }
}

void ShipGameLayer::UpdateScore(int points) {
    _score += points;
}

void ShipGameLayer::SpawnObstacle() {
    if (!_gameRunning) return;
    Entity* obstacle = GetPooledObstacle();
    if (!obstacle) return; 

    // Get player ship X position
    float shipX = _playerShipPosition.x;
    // Add a small random offset to X for variety
    float randomOffset = static_cast<float>((rand() % 100) - 50) / 10.0f; 
    float spawnX = shipX + randomOffset;
    float spawnY = 12.0f; // Above the visible area
    float spawnZ = _playerShipPosition.z; // Same Z as ship
    Transform* obstacleTransform = obstacle->GetComponent<Transform>();
    obstacleTransform->position = glm::vec3(spawnX, spawnY, spawnZ);
    float scaleX = 0.5f + static_cast<float>(rand() % 100) / 100.0f;
    float scaleY = 0.5f + static_cast<float>(rand() % 100) / 100.0f;
    obstacleTransform->scale = glm::vec3(scaleX, scaleY, 0.5f);
    ColliderComponent* obstacleCollider = obstacle->GetComponent<ColliderComponent>();
    obstacleCollider->SetCollider<BoxCollider>(obstacleTransform->scale);
    RigidBody* obstacleRb = obstacle->GetComponent<RigidBody>();


    // Set a random slow rotation in a random axis
    float axisRand = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float speedRand = 10.0f + static_cast<float>(rand() % 30); 

    glm::vec3 rotAxis(0.0f);
    if (axisRand < 0.33f) {
        rotAxis.x = 1.0f; 
        rotAxis.y = 1.0f; 
    }
    else if (axisRand < 0.66f) {
        rotAxis.y = 1.0f; 
        rotAxis.z = 1.0f; 
    }
    else{
        rotAxis.z = 1.0f; 
        rotAxis.x = 1.0f;
    }
    float rotSpeed = speedRand * (rand() % 2 == 0 ? 1.0f : -1.0f); // random direction
    obstacleTransform->rotation = glm::vec3(0.0f);
    obstacleRb->SetAngularVelocity(rotAxis * glm::radians(rotSpeed));
    obstacle->SetName("active"); 


    obstacleRb->SetAffectedByGravity(false);
    obstacleRb->SetMass(1.0f);
    obstacleRb->SetLinearVelocity(glm::vec3(0.0f, -_gameSpeed, 0.0f));

    

    // Reset script state
    Script* script = obstacle->GetComponent<Script>();
    if (script) {
        auto obsScript = std::dynamic_pointer_cast<ObstacleScript>(script->GetScript());
        if (obsScript) obsScript->Reset(_gameSpeed);
    }

    // Reset material color if needed
    MeshRenderable* renderable = obstacle->GetComponent<MeshRenderable>();
    if (renderable && renderable->GetMaterial()) {
        renderable->GetMaterial()->SetAmbient(glm::vec3(0.8f, 0.1f, 0.1f));
        renderable->GetMaterial()->SetDiffuse(glm::vec3(0.9f, 0.2f, 0.2f));
    }
    Tag* obstacleTag = obstacle->GetComponent<Tag>();
    if (obstacleTag) obstacleTag->SetTag("obstacle");
}