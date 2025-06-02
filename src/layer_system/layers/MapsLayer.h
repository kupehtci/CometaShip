//
// Created by Daniel Laplana Gimeno on 18/4/25.
//

#ifndef COMETA_MAPSLAYER_H
#define COMETA_MAPSLAYER_H

#include <iostream>
#include <render/Material.h>

#include "layer_system/Layer.h"
#include "render/Texture.h"
#include "render/Camera.h"

class MapsLayer : public Layer{

private:
    Camera _camera;
    // Material _mat;

public:
    MapsLayer();
    ~MapsLayer() override;

public:
    void Init() override;
    void Update() override;
    void Close() override;

    void HandleEvent(Event& event) override;
};

#endif //COMETA_MAPSLAYER_H
