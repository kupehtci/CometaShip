#ifndef COMETA_LAYER_H
#define COMETA_LAYER_H

#include <string>

#include "Event.h"

class Layer{
    // friend class Onion; 

protected: 
    std::string _name;

public:
    virtual ~Layer() = default;

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Close() = 0;

    virtual void HandleEvent(Event& event) = 0;

    std::string GetName(){ return _name; }
}; 

// // Implement destructor for the class
// // So can be called from derived classes
// Layer::~Layer() = default;

#endif
