#include "Onion.h"

void Onion::Init(){
    for(Layer* layer : _layers){
        layer->Init();
    }
}

void Onion::Update(){
    // Update backwards in order to render correctly
    for (auto it = _layers.rbegin(); it != _layers.rend(); ++it)
    {
        (*it)->Update();
    }
}

void Onion::Close(){
    for(Layer* layer : _layers){
        layer->Close();
    }
}

// --------- Layer management ---------

void Onion::PushLayer(Layer* layer){
    _layers.push_back(layer);
}

void Onion::PopLayer(Layer* layer){
    _layers.erase(
        std::remove(_layers.begin(), _layers.end(), layer), 
        _layers.end()
    );
}