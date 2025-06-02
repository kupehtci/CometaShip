//
// Created by Daniel Laplana Gimeno on 1/3/25.
//

#include "MaterialLayer.h"
#include "render/Renderer.h"

//
// Created by Daniel Laplana Gimeno on 1/3/25.
//

#include "CometaLayer.h"
#include "render/Renderer.h"

#include "render/Material.h"

MaterialLayer::MaterialLayer() {

}

MaterialLayer::~MaterialLayer(){

}

void MaterialLayer::Init() {
    std::cout << "Cometa layer init" << std::endl;

    _texture0 = new Texture("./resources/macos_example.jpg");
    _camera = Camera();
}

void MaterialLayer::Update(){
    // create a shader and example drawing elements
    // ALso update the uViewProjection used for generating the neccesary matrices for the camera in the shader

    Shader* mainShader = new Shader("Main Shader", "src/render/shaders/vertex_shader_coords_normals.vert", "src/render/shaders/material_shader.frag");
    mainShader->Bind();

    SimpleMaterial mat = SimpleMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(0.5f, 0.5f, 0.5f),  2.0f);


    // TODO: Remain to set light position
    mainShader->SetFloat3("material.color", mat.GetColor());
    mainShader->SetFloat3("material.ambient", mat.GetAmbient());
    mainShader->SetFloat3("material.diffuse", mat.GetDiffuse());
    mainShader->SetFloat3("material.specular", mat.GetSpecular());
    mainShader->SetFloat("material.shininess", mat.GetShininess());

    glm::vec3 lightPosition = glm::vec3(glm::cos(glfwGetTime()), glm::cos(glfwGetTime()) , -2.0f);

    mainShader->SetFloat3("light.position", lightPosition);
    mainShader->SetFloat3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f)/*glm::vec3(0.2f, 0.2f, 0.2f)*/);
    mainShader->SetFloat3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    mainShader->SetFloat3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    //float vertices[] = {
    //        // positions        //normals           // colors           // texture coords
    //        0.5f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    //        0.5f, -0.5f, 0.0f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    //        -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    //        -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
    //};

    //unsigned int indices[] = {
    //        0, 1, 3,   // first triangle
    //        1, 2, 3,    // second triangle
    //};

    float vertices[] = {
        // Front face
        // positions          // normals           // colors            // texture coords
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

         // Top face
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,

          // Bottom face
          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
           0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,

          // Right face
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
           0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
           0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,

           // Left face
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
           -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
           -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0,  1,  2,    2,  3,  0,   // Front
        4,  5,  6,    6,  7,  4,   // Back
        8,  9,  10,   10, 11, 8,   // Top
        12, 13, 14,   14, 15, 12,  // Bottom
        16, 17, 18,   18, 19, 16,  // Right
        20, 21, 22,   22, 23, 20   // Left
    };


    _texture0->Bind(0);

    VertexArray vArray0 = VertexArray();
    vArray0.Bind();

    VertexBuffer vBuffer0 = VertexBuffer(vertices, sizeof(vertices));
    IndexBuffer iBuffer0 = IndexBuffer(indices, sizeof(indices));


    LayoutBuffer layoutBuffer = {
            {0, DataType::Float3, "aPos"},
            {1, DataType::Float3, "aNormal"},
            {2, DataType::Float3, "aColor"},
            {3, DataType::Float2, "aTexCoord"}
    };

    layoutBuffer.Build();
    layoutBuffer.Bind();

    // Update camera and its proyection
    _camera.OnUpdate();
    mainShader->SetMatrix4("uViewProjection", _camera.GetViewProyection());
    mainShader->SetFloat3("uViewPos", _camera.GetPosition());                    // Set the view position for the fragment shader


    glm::mat4 modelRotated = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    mainShader->SetMatrix4("uModel", modelRotated);

    vArray0.Bind();

    _texture0->Bind(0);
    mainShader->Bind();
    mainShader->SetInt("ourTexture", 0);         // glUniform1i(glGetUniformLocation(mainShader.GetShaderUID(), "ourTexture"), 0);

    vArray0.Bind();

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

    mainShader->Unbind();


    // --------- Draw LIGHT POINT ---------

    Shader* lightShader = new Shader("Light Shader", "src/render/shaders/light_shader.vert", "src/render/shaders/light_shader.frag");
    lightShader->Bind();

    lightShader->SetMatrix4("uViewProjection", _camera.GetViewProyection());
    glm::mat4 lightPosMatrix = glm::translate(glm::mat4(1.0f), lightPosition);
    lightPosMatrix = glm::scale(lightPosMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

    lightShader->SetMatrix4("uModel", lightPosMatrix);

    vArray0.Bind();

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

    lightShader->Unbind();

    // --------- END OF DRAWING LIGHT POINT ---------
}

void MaterialLayer::Close() {
    std::cout << "Cometa layer close" << std::endl;
}

void MaterialLayer::HandleEvent(Event& event){

}
