//
// Created by Daniel Laplana Gimeno on 22/4/25.
//

#ifndef MESH_H
#define MESH_H

#include <vector>

#include "render/VertexArray.h"

class Mesh {
private:
    std::shared_ptr<VertexArray> _vao = nullptr;

    std::vector<float> _vertices;
    uint32_t _numVertices = 0;

    std::vector<uint32_t> _indices;
    uint32_t _numIndices = 0;

public:
    Mesh();
    ~Mesh() = default;

    // ------------ CUSTOM MESH METHODS ------------

    void AddVertices(float* vertices, uint32_t numVertices);
    void AddIndices(uint32_t* indices, uint32_t numIndices);

    void Build();
    void Draw();

    void Bind();
    void Unbind();

    // ------------ MESH CREATION METHODS ------------

    static std::shared_ptr<Mesh> CreateBox();

    static std::shared_ptr<Mesh> CreateSphere(unsigned int sectorCount = 36, unsigned int stackCount = 18, float radius = 0.5f);

    static std::shared_ptr<Mesh> CreatePlane();

    // ------------ DEBUG ------------

    void Debug();

    // ------------ GETTERS AND SETTERS ------------

    std::shared_ptr<VertexArray>& GetVertexArray() { return _vao; }
    [[nodiscard]] uint32_t GetNumVertices() const { return _numVertices; }
    [[nodiscard]] uint32_t GetNumIndices() const { return _numIndices; }

    void SetLayoutBuffer(const LayoutBuffer& layoutBuffer) const {_vao->SetLayoutBuffer(layoutBuffer);}
};



#endif //MESH_H
