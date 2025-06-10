// Created by Daniel Laplana Gimeno on 18/4/25.
//

#include "MapsLayer.h"

//
#include <layer_system/EventBus.h>
#include <physics/Collider.h>

#include "render/Renderer.h"
#include "render/Shader.h"
#include "render/Mesh.h"

#include "world/Entity.h"
#include "world/WorldManager.h" // #include "world/World.h"
#include "world/Components.h"

#include "input/Input.h"

#include "world/TestScript.h"


MapsLayer::MapsLayer()
{
    _name = "MapsLayer";
}

MapsLayer::~MapsLayer()
{

}

void MapsLayer::Init()
{
    float vertices[] = {
        // Front face
        // positions          // normals           // colors            // texture coords
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,

        // Right face
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,

        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,  1,  2,    2,  3,  0,   // Front
        4,  5,  6,    6,  7,  4,   // Back
        8,  9,  10,   10, 11, 8,   // Top
        12, 13, 14,   14, 15, 12,  // Bottom
        16, 17, 18,   18, 19, 16,  // Right
        20, 21, 22,   22, 23, 20   // Left
    };

    _camera = Camera();

    WorldManagerRef->CreateWorld(0);
    WorldManagerRef->SetCurrentWorld(0);
    std::shared_ptr<World> world0 = WorldManagerRef->GetWorld(0); // World();
    world0->SetCamera(&_camera);

    Entity* ent0 = world0->CreateEntity("Entity0");
    ent0->GetComponent<Transform>()->position = glm::vec3(0.0f, 3.0f, -7.0f);

    MeshRenderable* ent0Renderable =  ent0->CreateComponent<MeshRenderable>();

    RigidBody* ent0Rb = ent0->CreateComponent<RigidBody>();
    ent0Rb->SetAffectedByGravity(false);

    ColliderComponent* ent0Collider = ent0->CreateComponent<ColliderComponent>();
    ent0Collider->SetCollider<BoxCollider>(glm::vec3(0.5f, 0.5f, 0.5f));

    DirectionalLight* dir_light = ent0->CreateComponent<DirectionalLight>();
    std::cout << "Directional light direction: (" << dir_light->GetDirection().x << " , " << dir_light->GetDirection().y << " , " << dir_light->GetDirection().z << ")" <<std::endl;

    std::shared_ptr<Material> material0 = std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f),
                                    glm::vec3(1.0f, 0.5f, 0.31f),
                                    glm::vec3(1.0f, 0.5f, 0.31f),
                                    glm::vec3(0.5f, 0.5f, 0.5f),
                                    64.0f,
                                    /*"resources/bricks_diffuse_map.jpg"*/ "resources/white.jpg",
                                    "resources/white.jpg",
                                    "resources/black.jpg");

    material0->LoadShader("Main Shader",
        "src/render/shaders/blinn_phong_shader.vert",
        "src/render/shaders/blinn_phong_shader.frag");
    ent0Renderable->SetMaterial(material0);

    std::shared_ptr<Mesh> mesh0 = std::make_shared<Mesh>();
    mesh0->AddVertices(vertices, sizeof(vertices) / sizeof(float));
    mesh0->AddIndices(indices, sizeof(indices) / sizeof(unsigned int));
    mesh0->SetLayoutBuffer({
        {0, DataType::Float3, "aPos"},
        {1, DataType::Float3, "aNormal"},
        {2, DataType::Float3, "aColor"},
        {3, DataType::Float2, "aTexCoord"}
        });
    mesh0->Build();
    ent0Renderable->SetMesh(mesh0);

    Script* script = ent0->CreateComponent<Script>();
    script->Attach<TestScript>("Hello");

    // --------- Other entity same as ent0 ---------

    Entity* ent1 = world0->CreateEntity("Entity1");
    ent1->GetComponent<Transform>()->position = glm::vec3(0.0f, 0.0f, -7.0f);
    ColliderComponent* ent1Collider = ent1->CreateComponent<ColliderComponent>();
    
    ent1Collider->SetCollider<BoxCollider>(glm::vec3(0.5f, 0.5f, 0.5f));

    auto* ent1Renderable = ent1->CreateComponent<MeshRenderable>();

    ent1Renderable->SetMaterial(material0);
    ent1Renderable->SetMesh(mesh0);

    // ENTITY 2
    Entity* ent2 = world0->CreateEntity("Entity2");
    ent2->GetComponent<Transform>()->position = glm::vec3(2.0f, 0.0f, -7.0f);
    ColliderComponent* ent2Collider = ent2->CreateComponent<ColliderComponent>();

    ent2Collider->SetCollider<BoxCollider>(glm::vec3(0.5f, 0.5f, 0.5f));

    auto* ent2Renderable = ent2->CreateComponent<MeshRenderable>();

    ent2Renderable->SetMaterial(material0);
    ent2Renderable->SetMesh(mesh0);

    // ENTITY 3

    Entity* ent3 = world0->CreateEntity("Entity3");
    ent3->GetComponent<Transform>()->position = glm::vec3(-2.0f, 0.0f, -7.0f);
    ColliderComponent* ent3Collider = ent3->CreateComponent<ColliderComponent>();

    ent3Collider->SetCollider<BoxCollider>(glm::vec3(0.5f, 0.5f, 0.5f));

    auto* ent3Renderable = ent3->CreateComponent<MeshRenderable>();

    ent3Renderable->SetMaterial(material0);
    ent3Renderable->SetMesh(mesh0);

    // --------- Light entity ---------
    Entity* ptlight0 = world0->CreateEntity("Light Point 1");
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

    // OTHER LIGHT ENTITY

    // Entity* ptLight2 = world0->CreateEntity("Light Point 2");
    // ptLight2->CreateComponent<PointLight>();
    // MeshRenderable* ptLight2Renderable = ptLight2->CreateComponent<MeshRenderable>();
    //
    // std::shared_ptr<Material> material2 = std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f),
    //                                 glm::vec3(1.0f, 0.5f, 0.31f),
    //                                 glm::vec3(1.0f, 0.5f, 0.31f),
    //                                 glm::vec3(0.5f, 0.5f, 0.5f),
    //                                 2.0f,
    //                                 "resources/white.jpg",
    //                                 "resources/white.jpg",
    //                                 "resources/black.jpg");
    //
    // material2->LoadShader("Point light 2 shader","src/render/shaders/light_shader.vert", "src/render/shaders/light_shader.frag");
    //
    // ptLight2Renderable->SetMesh(Mesh::CreateSphere());
    // ptLight2Renderable->SetMaterial(material2);
    // ptLight2->GetComponent<Transform>()->position = glm::vec3(0.0f, 1.0f, 5.0f);
    // ptLight2->GetComponent<Transform>()->scale = glm::vec3(0.2f, 0.2f, 0.2f);


    // ------------ Floor entity ------------
    Entity* floor = world0->CreateEntity("Floor");
    floor->GetComponent<Transform>()->position = glm::vec3(0.0f, -2.0f, -7.0f);
    floor->GetComponent<Transform>()->rotation = glm::vec3(0.0f, 90.0f, 0.0f);
    floor->GetComponent<Transform>()->scale = glm::vec3(20.0f, 0.0f, 20.0f);

    MeshRenderable* floorRenderable = floor->CreateComponent<MeshRenderable>();
    floorRenderable->SetMesh(Mesh::CreatePlane());

    std::shared_ptr<Material> floorMaterial = std::make_shared<Material>(glm::vec3(1.0f, 1.0f, 1.0f),
                                    glm::vec3(1.0f, 0.5f, 0.31f),
                                    glm::vec3(1.0f, 0.5f, 0.31f),
                                    glm::vec3(0.5f, 0.5f, 0.5f),
                                    64.0f,
                                    /*"resources/bricks_diffuse_map.jpg"*/ "resources/white.jpg",
                                    "resources/white.jpg",
                                    "resources/black.jpg");

    floorMaterial->LoadShader("Main Shader","src/render/shaders/blinn_phong_shader.vert", "src/render/shaders/blinn_phong_shader.frag");
    floorRenderable->SetMaterial(floorMaterial);


    // Event bus subscription
    EventBus::GetInstancePtr()->Subscribe(EventType::COMETA_KEY_PRESS_EVENT, this);
    EventBus::GetInstancePtr()->Subscribe(EventType::COMETA_KEY_RELEASE_EVENT, this);
}

void MapsLayer::Update()
{

}


void MapsLayer::Close()
{

}

void MapsLayer::HandleEvent(Event& event){
    std::cout << "MapsLayer::HandleEvent" << std::endl;
    std::cout << "EventType: " << event.GetEventType() << std::endl;
    if (event.GetEventType() == COMETA_KEY_PRESS_EVENT)
    {
        std::cout << "MAPS LAYER handled key press: " << dynamic_cast<KeyPressEvent&>(event).GetKey() << std::endl;
        event.SetHandled();
    }

    switch (event.GetEventType())
    {
    case COMETA_MOUSE_BUTTON_PRESS_EVENT:
        std::cout << "MAPS LAYER handled key press: " << dynamic_cast<KeyPressEvent&>(event).GetKey() << std::endl;
        event.SetHandled();
        break;
    case COMETA_MOUSE_BUTTON_RELEASE_EVENT:
        std::cout << "MAPS LAYER handled key release: " << dynamic_cast<KeyPressEvent&>(event).GetKey() << std::endl;
        event.SetHandled();
        break;
    default:
        break;
    }
}
