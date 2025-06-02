#ifndef AURAGL_COMETALAYER_H
#define AURAGL_COMETALAYER_H

#include <iostream>
#include "layer_system/Layer.h"
#include "render/Texture.h"
#include "render/Camera.h"

class CometaLayer : public Layer{
private:
    Texture* _texture0;
    Camera _camera;
public:
    CometaLayer();
    ~CometaLayer() override;

    void Init() override;
    void Update() override;
    void Close() override;

    void HandleEvent(Event& event) override;
};


#endif //AURAGL_COMETALAYER_H
