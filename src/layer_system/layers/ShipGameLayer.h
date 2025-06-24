#pragma once

#include "layer_system/Layer.h"
#include "render/Camera.h"

class ShipGameLayer : public Layer {
public:
    ShipGameLayer();
    ~ShipGameLayer();

    void Init() override;
    void Update() override;
    void Close() override;
    void HandleEvent(Event& event) override;

private:
    enum class GameState {
        MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };

    void InitializeGameWorld();
    void InitImGui();
    void SetupLightsAndEnvironment();
    void SpawnObstacle();
    void ResetGame();
    void UpdateScore(int points);

    // Menu-related methods
    void RenderMenu();
    void RenderPauseMenu();
    void RenderGameOverScreen();
    void HandleMenuInput(int key);
    void StartGame();
    void ExitGame();
    void ShowControls() { _showControls = true; }
    void ShowHighScores() { _showHighScores = true; }

    Camera _camera;
    bool _gameRunning = false;
    float _gameSpeed = 1.0f;
    float _obstacleSpawnTimer = 0.0f;
    float _obstacleSpawnInterval = 2.0f;
    int _score = 0;
    uint32_t _playerShipId;

    // Menu state
    GameState _currentState = GameState::MENU;
    bool _showControls = false;
    bool _showHighScores = false;
    int _selectedMenuItem = 0;
    static const int NUM_MENU_ITEMS = 4;
    unsigned int _obstacleCounter = 0; 

    // Objects properties
    glm::vec3 _playerShipPosition = glm::vec3(0.0f, -1.0f, -5.0f);
};