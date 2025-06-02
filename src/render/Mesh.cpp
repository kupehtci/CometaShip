//
// Created by Daniel Laplana Gimeno on 22/4/25.
//

#include "Mesh.h"

// TODO: Delete this three references

// ------------ CONSTRUCTOR AND DESTRUCTOR ------------

Mesh::Mesh()
{
    _vao = std::make_shared<VertexArray>();
}


// ------------ CUSTOM MESH METHODS ------------

void Mesh::AddVertices(float* vertices, uint32_t numVertices)
{
    _vertices.insert(_vertices.end(), vertices, vertices + numVertices);
    _numVertices += numVertices;
}

void Mesh::AddIndices(uint32_t* indices, uint32_t numIndices)
{
    _indices.insert(_indices.end(), indices, indices + numIndices);
    _numIndices += numIndices;
}


void Mesh::Build()
{
    // std::cout << "################# MESH BUILD METHOD #################" << std::endl;

    // VertexArray vao = VertexArray();
    _vao->CreateVertexBuffer(_vertices.data(), _numVertices * sizeof(float));
    _vao->CreateIndexBuffer(_indices.data(), _numIndices * sizeof(uint32_t));

    // _vao->SetLayoutBuffer({
    //         {0, DataType::Float3, "aPos"},
    //         {1, DataType::Float3, "aNormal"},
    //         {2, DataType::Float3, "aColor"},
    //         {3, DataType::Float2, "aTexCoord"}
    // });
    _vao->GetLayoutBuffer().Bind();

    _vao->Bind();

    // std::cout << "################# END OF MESH BUILD METHOD #################" << std::endl;
}

void Mesh::Draw()
{
    _vao->Bind();
    glDrawElements(GL_TRIANGLES, _numVertices, GL_UNSIGNED_INT, 0);
}


void Mesh::Bind()
{
    _vao->Bind();
}

void Mesh::Unbind()
{
    _vao->Unbind();
}

// ------------ MESH CREATION METHODS ------------

// static
std::shared_ptr<Mesh> Mesh::CreateBox()
{
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

    std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>();
    boxMesh->AddVertices(vertices, sizeof(vertices) / sizeof(float));
    boxMesh->AddIndices(indices, sizeof(indices) / sizeof(unsigned int));
    boxMesh->SetLayoutBuffer({
        {0, DataType::Float3, "aPos"},
        {1, DataType::Float3, "aNormal"},
        {2, DataType::Float3, "aColor"},
        {3, DataType::Float2, "aTexCoord"}
        });
    boxMesh->Build();

    return boxMesh;
}

// static
std::shared_ptr<Mesh> Mesh::CreateSphere(unsigned int sectorCount, unsigned int stackCount, float radius)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const float PI = 3.14159265359f;

    for (unsigned int i = 0; i <= stackCount; ++i) {
        float stackAngle = PI / 2 - i * (PI / stackCount);
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * (2 * PI / sectorCount);

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            // Position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Normal
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            // Color (you can change this)
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);

            // Texture Coords
            float u = (float)j / sectorCount;
            float v = (float)i / stackCount;
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    for (unsigned int i = 0; i < stackCount; ++i) {
        for (unsigned int j = 0; j < sectorCount; ++j) {
            int first = (i * (sectorCount + 1)) + j;
            int second = first + sectorCount + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>();
    sphereMesh->AddVertices(vertices.data(), vertices.size());
    sphereMesh->AddIndices(indices.data(), indices.size());
    sphereMesh->SetLayoutBuffer({
        {0, DataType::Float3, "aPos"},
        {1, DataType::Float3, "aNormal"},
        {2, DataType::Float3, "aColor"},
        {3, DataType::Float2, "aTexCoord"}
    });
    sphereMesh->Build();

    return sphereMesh;
}

std::shared_ptr<Mesh> Mesh::CreatePlane()
{
    float vertices[] = {
        // positions          // normals          // texture coords
        -0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
         0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::shared_ptr<Mesh> planeMesh = std::make_shared<Mesh>();
    planeMesh->AddVertices(vertices, sizeof(vertices) / sizeof(float));
    planeMesh->AddIndices(indices, sizeof(indices) / sizeof(unsigned int));
    planeMesh->SetLayoutBuffer({
        {0, DataType::Float3, "aPos"},
        {1, DataType::Float3, "aNormal"},
        {2, DataType::Float2, "aTexCoord"}
    });
    planeMesh->Build();

    return planeMesh;
}

// --------- UTILS ----------

void Mesh::Debug()
{
    std::cout << "Vertices: " << _numVertices << std::endl;
    for (int i = 0 ; i < _numVertices ; i++)
    {
        std::cout << "Vertex ["<< i << "] : " << _vertices[i] << std::endl;
    }

    std::cout << "Indices: " << _numIndices << std::endl;
    for (int i = 0 ; i < _numIndices ; i++)
    {
        std::cout << "Indices [" << i << "] : " << _indices[i] << std::endl;
    }
}