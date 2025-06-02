#ifndef COMETA_ONION_H
#define COMETA_ONION_H

#include <vector>

#include "Layer.h"
#include "EventBus.h"

class Onion {

private: 
    std::vector<Layer*> _layers;
    // const EventBus* _eventBus;

public: 
    Onion(){
        _layers = std::vector<Layer*>();
    }

    void Init();
    void Update(); 
    void Close(); 

    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);

    std::vector<Layer*>::iterator begin() {return _layers.begin();}
    std::vector<Layer*>::iterator end() { return _layers.end(); }

    // Get the Event Bus to be able to subscribe and publish events
    // const EventBus* GetEventBus() const { return _eventBus; }
}; 

#endif