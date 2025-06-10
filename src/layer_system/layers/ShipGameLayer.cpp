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
    
    Entity* playerShip = gameWorld->CreateEntity("PlayerShip");
    _playerShipId = playerShip->GetUID();
    

    Transform* shipTransform = playerShip->GetComponent<Transform>();
    shipTransform->position = glm::vec3(0.0f, 0.0f, -5.0f); // Center of screen
    shipTransform->scale = glm::vec3(1.0f, 0.2f, 0.5f); // Flat, wide ship
    
    // Add collider to player ship
    ColliderComponent* shipCollider = playerShip->CreateComponent<ColliderComponent>();
    shipCollider->SetCollider<BoxCollider>(glm::vec3(1.0f, 0.2f, 0.5f)); // Match the scale
    
    // Add rigidbody to player ship
    RigidBody* shipRb = playerShip->CreateComponent<RigidBody>();
    shipRb->SetMass(1.0f);
    shipRb->SetAffectedByGravity(false);
    
    // Add renderable component to player ship
    MeshRenderable* shipRenderable = playerShip->CreateComponent<MeshRenderable>();
    
    // Create material for the ship
    std::shared_ptr<Material> shipMaterial = std::make_shared<Material>(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.1f, 0.1f, 0.8f),
        glm::vec3(0.2f, 0.2f, 0.9f),
        glm::vec3(0.8f, 0.8f, 1.0f),
        32.0f,
        "resources/rocket_cometa.png",
        "resources/white.jpg",
        "resources/black.jpg"
    );
    
    shipMaterial->LoadShader("Main Shader",
        "src/render/shaders/blinn_phong_shader.vert",
        "src/render/shaders/blinn_phong_shader.frag");

    shipRenderable->SetMaterial(shipMaterial);
    
    // Create a box mesh for the ship
    shipRenderable->SetMesh(Mesh::CreateBox());
    
    // Add script component to player ship
    Script* shipScript = playerShip->CreateComponent<Script>();
    shipScript->Attach<ShipScript>();


    // Add tag component to identify the player
    Tag* shipTag = playerShip->CreateComponent<Tag>();
    shipTag->SetTag("player");
    
    // Create a directional light
    Entity* lightEntity = gameWorld->CreateEntity("DirectionalLight");
    DirectionalLight* dirLight = lightEntity->CreateComponent<DirectionalLight>();
    dirLight->SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
    
    // Create a floor/background
    Entity* floor = gameWorld->CreateEntity("Floor");
    floor->GetComponent<Transform>()->position = glm::vec3(0.0f, -2.0f, -10.0f);
    floor->GetComponent<Transform>()->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    floor->GetComponent<Transform>()->scale = glm::vec3(20.0f, 0.1f, 40.0f);
    
    MeshRenderable* floorRenderable = floor->CreateComponent<MeshRenderable>();
    floorRenderable->SetMesh(Mesh::CreateBox());
    
    std::shared_ptr<Material> floorMaterial = std::make_shared<Material>(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.3f, 0.3f, 0.3f),
        glm::vec3(0.1f, 0.1f, 0.1f),
        8.0f,
        "resources/isometric_cubes.jpg",
        "resources/white.jpg",
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

    _gameRunning = true;
}

void ShipGameLayer::Update() {
    if (!_gameRunning) return;
    
    float deltaTime = Time::GetDeltaTime();


    _camera.OnUpdate();

    _obstacleSpawnTimer += deltaTime;
    if (_obstacleSpawnTimer >= _obstacleSpawnInterval) {
        SpawnObstacle();
        _obstacleSpawnTimer = 0.0f;

        _gameSpeed += 0.001f;
        _obstacleSpawnInterval = std::max(0.5f, _obstacleSpawnInterval - 0.02f);
    }

    // Increase score
    UpdateScore(1);
}

void ShipGameLayer::Close() {
    _gameRunning = false;
}

void ShipGameLayer::HandleEvent(Event& event) {
    if (event.GetEventType() == COMETA_KEY_PRESS_EVENT) {
        KeyPressEvent& keyEvent = dynamic_cast<KeyPressEvent&>(event);
        int key = keyEvent.GetKey();

        if (key == GLFW_KEY_R) {
            ResetGame();
            event.SetHandled();
        }

        if (key == GLFW_KEY_P) {
            _gameRunning = !_gameRunning;
            event.SetHandled();
        }
    }
}

void ShipGameLayer::SpawnObstacle() {
    if (!_gameRunning) return;
    
    std::shared_ptr<World> gameWorld = WorldManagerRef->GetCurrentWorld();

    Entity* obstacle = gameWorld->CreateEntity(("Obstacle_" + std::to_string(_obstacleCounter)));
    _obstacleCounter++;
    
    float randomX = static_cast<float>(rand() % 10 - 5); // -5 to 5
    Transform* obstacleTransform = obstacle->GetComponent<Transform>();
    obstacleTransform->position = glm::vec3(randomX, 10.0f, -5.0f);
    
    float scaleX = 0.5f + static_cast<float>(rand() % 100) / 100.0f; // 0.5 to 1.5
    float scaleY = 0.5f + static_cast<float>(rand() % 100) / 100.0f; // 0.5 to 1.5
    obstacleTransform->scale = glm::vec3(scaleX, scaleY, 0.5f);

    ColliderComponent* obstacleCollider = obstacle->CreateComponent<ColliderComponent>();
    obstacleCollider->SetCollider<BoxCollider>(obstacleTransform->scale);

    RigidBody* obstacleRb = obstacle->CreateComponent<RigidBody>();
    obstacleRb->SetAffectedByGravity(false);
    obstacleRb->SetMass(1.0f);

    obstacleRb->SetLinearVelocity(glm::vec3(0.0f, -_gameSpeed, 0.0f));

    MeshRenderable* obstacleRenderable = obstacle->CreateComponent<MeshRenderable>();
    std::shared_ptr<Material> obstacleMaterial = std::make_shared<Material>(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.8f, 0.1f, 0.1f), // Red ambient
        glm::vec3(0.9f, 0.2f, 0.2f), // Red diffuse
        glm::vec3(1.0f, 0.6f, 0.6f), // Red specular
        16.0f,
        "resources/white.jpg",
        "resources/white.jpg",
        "resources/black.jpg"
    );
    
    obstacleMaterial->LoadShader("Obstacle Shader", 
        "src/render/shaders/blinn_phong_shader.vert", 
        "src/render/shaders/blinn_phong_shader.frag");
    
    obstacleRenderable->SetMaterial(obstacleMaterial);
    obstacleRenderable->SetMesh(Mesh::CreateBox());

    Script* obstacleScript = obstacle->CreateComponent<Script>();
    obstacleScript->Attach<ObstacleScript>(_gameSpeed);

    Tag* obstacleTag = obstacle->CreateComponent<Tag>();
    obstacleTag->SetTag("obstacle");
}

void ShipGameLayer::ResetGame() {
    _gameRunning = true;
    _score = 0;
    _obstacleSpawnTimer = 0.0f;
    _obstacleSpawnInterval = 2.0f;
    _gameSpeed = 5.0f;


    std::shared_ptr<World> gameWorld = WorldManagerRef->GetCurrentWorld();

    SparseSet<Entity>& entities = gameWorld->GetEntities();
    for (size_t i = 0; i < entities.Size(); i++) {
        Entity* entity = entities.Get(entities.GetDenseIndex(i));
        if (entity) {
            Tag* tag = entity->GetComponent<Tag>();
            if (tag && tag->GetTag() == "obstacle") {
                gameWorld->RemoveEntity(entity->GetUID());
            }
        }
    }

    Entity* playerShip = nullptr;
    for (size_t i = 0; i < entities.Size(); i++) {
        Entity* entity = entities.Get(entities.GetDenseIndex(i));
        if (entity && entity->GetUID() == _playerShipId) {
            playerShip = entity;
            break;
        }
    }
    
    if (playerShip) {
        Transform* shipTransform = playerShip->GetComponent<Transform>();
        shipTransform->position = glm::vec3(0.0f, 0.0f, -5.0f); 
        shipTransform->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

        RigidBody* shipRb = playerShip->GetComponent<RigidBody>();
        if (shipRb) {
            shipRb->SetLinearVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
            shipRb->SetAffectedByGravity(false);
            shipRb->SetMass(1.0f);
            shipRb->SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
        }
    }
}

void ShipGameLayer::UpdateScore(int points) {
    _score += points;
}