//
// Created by Daniel Laplana Gimeno on 28/4/25.
//

#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include "core/Singleton.h"
#include "world/World.h"



class WorldManager : public SingletonManager<WorldManager>{
private:
    //std::vector<std::shared_ptr<World>> _worlds;
    std::unordered_map<uint32_t, std::shared_ptr<World>> _worlds;
    std::shared_ptr<World> _currentWorld = nullptr;

public:
    WorldManager();
    ~WorldManager() override;

    // ------------ Singleton Manager methods ------------
    void Init() override;
    void Update() override;
    void Close() override;

    // ------------ World management methods ------------

    std::shared_ptr<World> CreateWorld(uint32_t index);
    std::shared_ptr<World> GetWorld(uint32_t index);
    std::shared_ptr<World> AddWorld(const std::shared_ptr<World>& world, uint32_t index);
    // void SetCurrentWorld(std::shared_ptr<World> world);

    void SetCurrentWorld(size_t index);
    std::shared_ptr<World> GetCurrentWorld();




public:
    // ------------ GETTERS ------------
};

#define WorldManagerRef WorldManager::GetInstancePtr()


#endif //WORLDMANAGER_H
