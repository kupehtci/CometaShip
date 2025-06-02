//
// Created by Daniel Laplana Gimeno on 1/3/25.
//

#include "CometaLayer.h"
#include "render/Renderer.h"

CometaLayer::CometaLayer() {

}

CometaLayer::~CometaLayer(){

}

void CometaLayer::Init() {
    std::cout << "Cometa layer init" << std::endl;

    _texture0 = new Texture("./resources/macos_example.jpg");
    _camera = Camera();
}

void CometaLayer::Update(){
    // create a shader and example drawing elements
    // ALso update the uViewProjection used for generating the neccesary matrices for the camera in the shader


    Shader* mainShader = nullptr; //Renderer::GetInstancePtr()->GetObjectShader();
    mainShader->Bind();

    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    };

    unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3,    // second triangle
    };


    _texture0->Bind(0);

    VertexArray vArray0 = VertexArray();
    vArray0.Bind();

    VertexBuffer vBuffer0 = VertexBuffer(vertices, sizeof(vertices));
    IndexBuffer iBuffer0 = IndexBuffer(indices, sizeof(indices));


    LayoutBuffer layoutBuffer = {
            {0, DataType::Float3, "_position"},
            {1, DataType::Float3, "_color"},
            {2, DataType::Float2, "_texCoord"}
    };

    layoutBuffer.Build();
    layoutBuffer.Bind();

    // Update camera and its proyection
    _camera.OnUpdate();
    mainShader->SetMatrix4("uViewProjection", _camera.GetViewProyection());


    glm::mat4 modelRotated = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)); // glm::mat4(1.0f); // glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    mainShader->SetMatrix4("uModel", modelRotated);

    vArray0.Bind();

    _texture0->Bind(0);

    mainShader->SetInt("ourTexture", 0);         // glUniform1i(glGetUniformLocation(mainShader.GetShaderUID(), "ourTexture"), 0);

    vArray0.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);


    mainShader->Unbind();
}

void CometaLayer::Close() {
    std::cout << "Cometa layer close" << std::endl;
}

void CometaLayer::HandleEvent(Event& event){

}
