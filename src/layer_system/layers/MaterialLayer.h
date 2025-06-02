#ifndef COMETA_MATERIAL_LAYER_H
#define COMETA_MATERIAL_LAYER_H

#include <iostream>
#include "layer_system/Layer.h"
#include "render/Texture.h"
#include "render/Camera.h"

class MaterialLayer : public Layer {
private:
    Texture* _texture0;
    Camera _camera;
public:
    MaterialLayer();
    ~MaterialLayer() override;

    void Init() override;
    void Update() override;
    void Close() override;

    void HandleEvent(Event& event) override;
};


#endif //COMETA_MATERIAL_LAYER_H
