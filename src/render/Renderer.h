
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE

#include "core/CometaDefaults.h"
#include "core/Singleton.h"
#include "render/Window.h"

#ifndef COMETA_RENDERER_H
#define COMETA_RENDERER_H

#include "render/FrameBuffer.h"
#include "render/CubeMapFrameBuffer.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include "render/Shader.h"
#include "render/Texture.h"
#include "render/Mesh.h"
#include "render/Material.h"
#include "render/Model.h"
#include "world/Components.h"

enum FACE_CULLING_MODE {
    FACE_CULLING_NONE = 0, 
    FACE_CULLING_CLOCKWISE = 1, 
    FACE_CULLING_COUNTER_CLOCKWISE = 2
};

class World;

class Renderer : public SingletonManager<Renderer>{
    friend class Application;

private:
    Window* _window;

    bool _depthCulling = true;
    FACE_CULLING_MODE _faceCullingMode = FACE_CULLING_MODE::FACE_CULLING_CLOCKWISE; 
    
    // Shadow mapping properties
    static const unsigned int SHADOW_MAP_WIDTH = 1024;
    static const unsigned int SHADOW_MAP_HEIGHT = 1024;
    constexpr static const float POINT_LIGHT_FAR_PLANE = 25.0f;
    
    // Directional light shadow mapping
    FrameBuffer* _shadowFrameBuffer = nullptr;
    std::shared_ptr<Shader> _shadowMapShader = nullptr;


    // Point light shadow mapping
    CubeMapFrameBuffer* _pointShadowFrameBuffer = nullptr;
    std::shared_ptr<Shader> _pointShadowMapShader = nullptr;

    std::vector<MeshRenderable> _renderables;

public:
    Renderer();
    ~Renderer() override;

public:
    void Init() override;
    void Update() override;
    void Close() override;

    void Render();

    void AddRenderable(const MeshRenderable& renderable);

    [[nodiscard]] Window* GetWindow() const { return _window; }

    // TODO: Remain to implement
    inline void SetDepthCulling(bool value) { _depthCulling = value;  }
    inline void SetFaceCulling(FACE_CULLING_MODE value) {
    }
};


#endif //COMETA_RENDERER_H
