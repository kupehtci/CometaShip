#ifndef COMETA_SHIPGAMELAYER_H
#define COMETA_SHIPGAMELAYER_H

#include <iostream>
#include "layer_system/Layer.h"
#include "render/Camera.h"

class ShipGameLayer : public Layer {
private:
    Camera _camera;
    
    // Game state tracking
    bool _gameRunning = false;
    int _score = 0;
    float _obstacleSpawnTimer = 0.0f;
    uint8_t _obstacleCounter = 0; 
    float _obstacleSpawnInterval = 2.0f; // Spawn a new obstacle every 2 seconds
    float _gameSpeed = 5.0f; // Base speed of obstacles moving toward player
    
    // Entity references
    uint32_t _playerShipId = 0;
    
public:
    ShipGameLayer();
    ~ShipGameLayer() override;

    void Init() override;
    void Update() override;
    void Close() override;

    void HandleEvent(Event& event) override;
    
    // Game specific methods
    void SpawnObstacle();
    void ResetGame();
    void UpdateScore(int points);
};

#endif //COMETA_SHIPGAMELAYER_H