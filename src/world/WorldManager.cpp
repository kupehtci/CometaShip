//
// Created by Daniel Laplana Gimeno on 28/4/25.
//

#include "WorldManager.h"
#include "ScriptSystem.h"
#include "core/Time.h"

WorldManager::WorldManager(){
    //this->_worlds = std::vector<std::shared_ptr<World>>(10, nullptr);
    _worlds = std::unordered_map<uint32_t, std::shared_ptr<World>>();
}
WorldManager::~WorldManager(){
    this->_worlds.clear();
}

// ------------ Singleton Manager methods ------------

void WorldManager::Init(){
    // std::cout << "WorldManager::Init()" << std::endl;
    
    // Initialize ScriptSystem
    if (_currentWorld) {
        ScriptManagerRef->InitScripts(_currentWorld.get());
    }
}
void WorldManager::Update(){
    // std::cout << "WorldManager::Update()" << std::endl;
    
    // Update scripts in the current world
    if (_currentWorld) {
        ScriptManagerRef->UpdateScripts(_currentWorld.get(), Time::GetDeltaTime());
    }
}
void WorldManager::Close(){
    // std::cout << "WorldManager::Close()" << std::endl;
    
    // Clean up ScriptSystem
    // ScriptManagerRef::Destroy();
}

// ------------ World management methods ------------

std::shared_ptr<World> WorldManager::CreateWorld(uint32_t index){
    std::shared_ptr<World> world = std::make_shared<World>();
    world->_uid = index;
    _worlds[index] = world;
    return world;
}

std::shared_ptr<World> WorldManager::GetWorld(uint32_t index){
    if (_worlds.find(index) == _worlds.end())
    {
        COMETA_MSG("[WORLD_MANAGER] World doesnt exist at index: ", index);
        return nullptr;
    }

    return _worlds[index];
}

std::shared_ptr<World> WorldManager::AddWorld(const std::shared_ptr<World>& world, uint32_t index)
{
    _worlds.insert({index, world});
    return _worlds[index];
}

// void WorldManager::SetCurrentWorld(std::shared_ptr<World> world)
// {
//     if (!_worlds.)
// }
void WorldManager::SetCurrentWorld(size_t index)
{
    if (_worlds.find(index) == _worlds.end())
    {
        COMETA_MSG("[WORLD_MANAGER] Cannot set as current a non-existing world at index: ", index);
        return;
    }
    _currentWorld = _worlds[index];

    if (_currentWorld) {
        ScriptManagerRef->InitScripts(_currentWorld.get());
    }
}

std::shared_ptr<World> WorldManager::GetCurrentWorld()
{
    if (_currentWorld == nullptr)
    {
        COMETA_WARNING("[WORLD_MANAGER] Cannot get current world as it has not been assigned");
        return nullptr;
    }
    return _currentWorld;
}