#include "World.h"

#include <iostream>

#include "world/Entity.h"
// #include "world/Components.h"
// #include "world/ComponentRegistry.h"

int World::worldInstanceCount = 0;

World::World()
{
    worldInstanceCount++;
    std::cout << "World::World() --> worldInstanceCount: " << worldInstanceCount << std::endl;
    _entities = SparseSet<Entity>();

    // TODO: Implement camera search between each one of the instances
}

World::~World()
{

}

/**
 * Create an entity and store in the world.
 * The entity is created together with a Transform
 * @param name name of the entity
 * @return Entity* pointer to the entity
 */
Entity* World::CreateEntity(const std::string& name)
{
    Entity newEntity = Entity(name);
    uint32_t newUid = newEntity.GetUID();
    newEntity._parentWorld = this;
    _entities.Add(newUid, newEntity);

    // Create an initial transform component for the entity
    newEntity.CreateComponent<Transform>();

    return _entities.Get(newUid);
}

/**
 * Delete an entity and its components associated
 * @param uid Unique ID of the Entity to remove from this world
 * @return bool that indicated the success in deleting the Entity. It will return false if Entity is not contained in this World or cannot delete.
 */
bool World::RemoveEntity(const uint32_t& uid)
{
    if (!_entities.Contains(uid))
    {
        COMETA_WARNING("[WORLD] Tried to delete entity that doesnt exist or its not contained in this world");
        return false;
    }

    _componentRegistry.GetStorage<Transform>().Pop(_uid);
    _componentRegistry.GetStorage<MeshRenderable>().Pop(_uid);
    _componentRegistry.GetStorage<SpriteRenderable>().Pop(_uid);
    _componentRegistry.GetStorage<PointLight>().Pop(_uid);
    _componentRegistry.GetStorage<DirectionalLight>().Pop(_uid);
    _componentRegistry.GetStorage<ColliderComponent>().Pop(_uid);
    _componentRegistry.GetStorage<RigidBody>().Pop(_uid);
    _componentRegistry.GetStorage<Script>().Pop(_uid);
    _componentRegistry.GetStorage<Tag>().Pop(_uid);

    _entities.Pop(uid);

    return true;
}


void World::DebugPrint(){
    std::cout << "=== WORLD DEBUG INFO ===" << std::endl;
    std::cout << "World instance: " << worldInstanceCount << std::endl;
    std::cout << "Number of entities: " << _entities.Size() << std::endl;
    
    // Print all entities with its components
    std::cout << "\n--- ENTITIES ---" << std::endl;
    for (size_t i = 0; i < _entities.Size(); i++) {
        Entity* entity = _entities.Get(_entities.GetDenseIndex(i));

        if (entity) {
            std::cout << "Processing entity: " << entity->GetUID() << std::endl;
            std::cout << "Entity UID: " << entity->GetUID() << ", Name: " << entity->GetName() << std::endl;

            if (entity->HasComponent<Transform>()) {
                Transform* transform = entity->GetComponent<Transform>();
                std::cout << "  - Transform: " 
                          << "Pos(" << transform->position.x << ", "
                          << transform->position.y << ", "
                          << transform->position.z << "), "
                          << "Rot(" << transform->rotation.x << ", " 
                          << transform->rotation.y << ", " 
                          << transform->rotation.z << "), "
                          << "Scale(" << transform->scale.x << ", " 
                          << transform->scale.y << ", " 
                          << transform->scale.z << ")" << std::endl;
            }

            if (entity->HasComponent<MeshRenderable>()) {
                std::cout << "  - MeshRenderable: Yes" << std::endl;
                MeshRenderable* renderable = entity->GetComponent<MeshRenderable>();
                if (renderable->GetMesh() != nullptr){
                    std::cout << "      - Has mesh" << std::endl;
                }

                if (renderable->GetMaterial() != nullptr){
                    std::cout << "      - Has material" << std::endl;
                }
            }

            if (entity->HasComponent<SpriteRenderable>()) {
                SpriteRenderable* sprite = entity->GetComponent<SpriteRenderable>();
                std::cout << "  - SpriteRenderable: Color(" 
                          << sprite->color.r << ", " 
                          << sprite->color.g << ", " 
                          << sprite->color.b << ", " 
                          << sprite->color.a << ")" << std::endl;
            }

            if (entity->HasComponent<ColliderComponent>()) {
                std::cout << "  - Collider: Yes" << std::endl;
            }

            if (entity->HasComponent<RigidBody>()) {
                std::cout << "  - RigidBody: Yes" << std::endl;
            }

            if (entity->HasComponent<Tag>()) {
                Tag* tag = entity->GetComponent<Tag>();
                std::cout << "  - Tag: " << tag->GetTag() << std::endl;
            }
            
            std::cout << std::endl;
        }
    }
    
    std::cout << "=== END WORLD DEBUG INFO ===" << std::endl;
}